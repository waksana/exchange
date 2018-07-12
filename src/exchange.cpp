/**
 * simple decentralized exchange
 * smart contract on eos
 *
 */

#include <exchange.hpp>

using namespace eosio;

void exchange::add_order(order_index* orders, account_name maker, asset quantity, uint128_t price) {
  if(quantity.amount > 0) {
    orders->emplace(maker, [&](auto& r) {
      r.id = orders->available_primary_key();
      r.price = price;
      r.quantity = quantity;
      r.maker = maker;
    });
  }
}
void exchange::test(account_name maker, asset quantity, uint128_t price) {
  print("bid scope:", N(bid));
  order_index orders(_self, N(bid));
  add_order(&orders, maker, quantity, price);
}

EOSIO_ABI(exchange, (test))
