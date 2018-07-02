/**
 * simple decentralized exchange
 * smart contract on eos
 *
 */

#include <exchange.hpp>

using namespace eosio;

namespace simple_decentralized_exchange {
  //buy
  void exchange::bid(account_name maker, extended_asset quantity, int64_t price) {
    //充值eos
    deposit(maker, ceil(exchange_to_eos(quantity, price)));

    //查找对手
    asks ask_orders(_self, quantity.contract);
    //查找所有卖价低于price的卖方按照价格时间排序
    order ask_order = find_ask(ask_orders, quantity, price);

    auto left = quantity;

    while(left.amount > 0 && ask_order != ask_orders.end()) {
      auto exchange_quantity = asset_min(ask_order.quantity, quantity);

      auto eos_exchange_quantity = exchange_to_eos(exchange_quantity, ask_order.price);

      withdraw(maker, exchange_quantity);
      withdraw(ask_order.maker, floor(eos_exchange_quantity));

      left = quantity - exchange_quantity;

      sub_ask(ask_order, exchange_quantity);

      //迭代
      ask_order = ask_order.next();
    }
    add_bid(maker, left, price);
  }

  void exchange::ask(account_name maker, extended_asset quantity, int64_t price) {

    deposit(maker, quantity);

    bids bid_orders(_self, quantity.contract);
    //查找所有买价高于price的买方
    order bid_order = find_bid(bid_orders, quantity, price);

    auto left = quantity;

    while(left.amount > 0 && bid_order != bid_orders.end()) {
      auto exchange_quantity = asset_min(bid_order.quantity, quantity);
      auto eos_exchange_quantity = exchange_to_eos(exchange_quantity, price);

      withdraw(maker, floor(eos_exchange_quantity));
      withdraw(bid_order.maker, exchange_quantity);

      left = quantity - exchange_quantity;

      sub_bid(bid_order, exchange_quantity);
    }
    add_ask(maker, quantity, price);
  }

  void exchange::deposit(account_name from, extended_asset quantity) {

    require_auth(from);

    auto sym = quantity.symbol;

    eosio_assert(sym.is_valid(), "invalid symbol name");

    action(
        permission_level{ from, N(active) },
        contract, N(transfer),
        std::make_tuple(from, _self, quantity, std::string("deposit"))
        ).send();

    add_balance(from, quantity, from);

  }

  void exchange::add_balance(account_name owner, asset value, account_name ram_payer) {
    accounts to_acnts(_self, owner);
    auto to = to_acnts.find(value.symbol.name());
    if(to == to_acnts.end()) {
      to_acnts.emplace(ram_payer, [&](auto& a) {
        a.balance = value;
        a.on_order = asset (int64_t 0, value.symbol);
      });
    }
    else {
      to_acnts.modify(to, 0, [&](auto& a) {
        a.balance += value;
      });
    }
  }

  void exchange::sub_balance(account_name owner, asset value) {
    accounts from_acnts(_self, owner);
    const auto& from = from_acnts.get( value.symbol.name(), "no balance object found" );
    eosio_assert(from.balance.amount >= value.amount, "overdrawn balance");
    from_acnts.modify(from, owner, [&]( auto& a ) {
      a.balance -= value;
    });
  }
}

EOSIO_ABI(simple_decentralized_exchange::exchange, (bid)(ask))
