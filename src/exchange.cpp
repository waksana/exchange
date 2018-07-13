/**
 * simple decentralized exchange
 * smart contract on eos
 *
 */

#include <exchange.hpp>

using namespace eosio;

void exchange::add_order(uint64_t scope, account_name maker, asset quantity, uint64_t price) {
  order_index orders(_self, scope);
  if(quantity.amount > 0) {
    orders.emplace(maker, [&](auto& r) {
      r.id = orders.available_primary_key();
      r.price = price;
      r.quantity = quantity;
      r.maker = maker;
    });
  }
}

void exchange::deposit(account_name contract, account_name user, asset quantity) {
  action(
      permission_level{user, N(active)},
      contract, N(transfer),
      std::make_tuple(user, _self, quantity, std::string("deposit"))
    ).send();
}

void exchange::withdraw(account_name contract, account_name user, asset quantity) {
  action(
      permission_level{_self, N(active)},
      contract, N(transfer),
      std::make_tuple(_self, user, quantity, std::string("withdraw"))
    ).send();
}

asset exchange::to_eos(asset quantity, uint64_t price, bool floor) {
  //do many check
  uint64_t amt = quantity.amount * price;

  int64_t p = (int64_t)quantity.symbol.precision();
  int64_t p10 = 1;

  while(p > 0) {
    p10 *= 10; --p;
  }

  uint64_t res = amt / p10;

  if(!floor && res * p10 != amt)
    res++;
  return asset(res, string_to_symbol(4, "EOS"));
}

asset exchange::asset_min(asset a, asset b) {
  if(a > b) return b;
  else return a;
}

void exchange::ask(account_name maker, asset quantity, uint64_t price) {
  //充值其他代币，卖
  deposit(token_contract, maker, quantity);

  //查找对手
  order_index bid_orders(_self, N(bid));
  //查找所有买价高于price的买方按照价格时间排序
  auto bid_orders_by_price = bid_orders.get_index<N(byprice)>();
  auto bid_it = bid_orders_by_price.lower_bound(price);

  auto left = quantity;
  asset maker_receive(0, eos_symbol);

  while(left.amount > 0 && bid_it != bid_orders_by_price.end()) {
    auto exchange_quantity = asset_min(bid_it->quantity, left);

    auto eos_exchange_quantity = to_eos(exchange_quantity, bid_it->price, true);

    maker_receive += eos_exchange_quantity;
    withdraw(token_contract, bid_it->maker, eos_exchange_quantity);

    left -= exchange_quantity;

    if(bid_it->quantity == exchange_quantity) {
      bid_it = bid_orders_by_price.erase(bid_it);
    }
    else {
      bid_orders_by_price.modify(bid_it, maker, [&](auto& r) {
        r.quantity -= exchange_quantity;
      });
      bid_it++;
    }
  }
  add_order(N(ask), maker, left, price);

  withdraw(N(eosio.token), maker, maker_receive);
}

void exchange::bid(account_name maker, asset quantity, uint64_t price) {
  //充值eos
  deposit(N(eosio.token), maker, to_eos(quantity, price, false));

  //查找对手
  order_index ask_orders(_self, N(ask));
  //查找所有卖价低于price的卖方按照价格时间排序
  auto ask_orders_by_price = ask_orders.get_index<N(byprice)>();
  auto ask_it = ask_orders_by_price.upper_bound(price);

  auto left = quantity;
  asset maker_receive(0, token_symbol);

  while(left.amount > 0 && ask_it != ask_orders_by_price.end()) {
    auto exchange_quantity = asset_min(ask_it->quantity, left);

    auto eos_exchange_quantity = to_eos(exchange_quantity, ask_it->price, true);

    maker_receive += exchange_quantity;
    withdraw(N(eosio.token), ask_it->maker, eos_exchange_quantity);

    left -= exchange_quantity;

    if(ask_it->quantity == exchange_quantity) {
      ask_it = ask_orders_by_price.erase(ask_it);
    }
    else {
      ask_orders_by_price.modify(ask_it, maker, [&](auto& r) {
        r.quantity -= exchange_quantity;
      });
      ask_it++;
    }
  }
  add_order(N(bid), maker, left, price);

  withdraw(token_contract, maker, maker_receive);
}

EOSIO_ABI(exchange, (bid))
