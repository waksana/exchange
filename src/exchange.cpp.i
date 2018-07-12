/**
 * simple decentralized exchange
 * smart contract on eos
 *
 */

#include <exchange.hpp>

using namespace eosio;

namespace simple_decentralized_exchange {
  void exchange::add_order(order_index orders, account_name maker, asset quantity, uint128_t price) {
    if(quantity.amount > 0) {
      orders.emplace(maker, [&](auto& r) {
        r.id = orders.available_primary_key();
        r.price = price;
        r.quantity = quantity;
        r.maker = maker;
      });
    }
  }
  /*
  void exchange::sub_order(order_index orders, iterator& it, asset quantity, account_name payer) {
    if(it->quantity.amount > quantity.amount) {
      orders.modify(it, payer, [&](auto& r) {
        r.quantity = r.quantity - quantity;
      });
    }
    else {
      orders.erase(it);
    }
  }
  */
  void exchange::testaddbid(account_name maker, asset quantity, uint128_t price) {
    print("bid scope:", N(bid));
    order_index orders(_self, N(bid));
    add_order(orders, maker, quantity, price);
  }
  /*
  //buy
  void exchange::bid(account_name maker, asset quantity, uint128_t price) {
    //充值eos
    deposit(maker, ceil(exchange_to_eos(quantity, price)));

    //查找对手
    asks ask_orders(_self, quantity.symbol.name());
    //查找所有卖价低于price的卖方按照价格时间排序
    auto ask_orders_by_price = ask_orders.get_index<N(byprice)>();
    auto ask_it = ask_orders_by_price.upper_bound(price);

    auto left = quantity;

    while(left.amount > 0 && ask_it != ask_orders_by_price.end()) {
      auto exchange_quantity = asset_min(ask_it->quantity, quantity);

      auto eos_exchange_quantity = exchange_to_eos(exchange_quantity, ask_it->price);

      withdraw(maker, exchange_quantity);
      withdraw(ask_it->maker, floor(eos_exchange_quantity));

      left = quantity - exchange_quantity;

      sub_ask(ask_it, exchange_quantity);

      //迭代
      ask_it++;
    }
    add_bid(maker, left, price);
  }

  void exchange::ask(account_name maker, extended_asset quantity, int64_t price) {

    deposit(maker, quantity);

    bids bid_orders(_self, quantity.symbol.name());
    //查找所有买价高于price的买方
    auto bid_orders_by_price = bid_orders.get_index<>(N(byprice));
    auto bid_it = bid_orders_by_price.lower_bound(price);

    auto left = quantity;

    while(left.amount > 0 && bid_it != bid_orders_by_price.end()) {
      auto exchange_quantity = asset_min(bid_it->quantity, quantity);

      //price变量低于所有买方价格所以取卖方price
      auto eos_exchange_quantity = exchange_to_eos(exchange_quantity, price);

      withdraw(maker, floor(eos_exchange_quantity));
      withdraw(bid_it->maker, exchange_quantity);

      left = quantity - exchange_quantity;

      sub_bid(bid_it, exchange_quantity);

      bid_it++;
    }
    add_ask(maker, left, price);
  }
  */
}

EOSIO_ABI(simple_decentralized_exchange::exchange, (testaddbid))

  /*

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
  */
