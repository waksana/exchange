/**
 * simple decentralized exchange
 * smart contract on eos
 *
 */

#pragma once

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>

using namespace eosio;

class exchange : public contract {
  public:
    using contract::contract;
    //@abi action
    void test(account_name maker, asset quantity, uint128_t price);
  private:
    //@abi table
    struct order {
      uint64_t id;
      uint128_t price;
      asset quantity;
      account_name maker;
      uint64_t primary_key() const { return id; }
      uint128_t get_price() const { return price; }
      EOSLIB_SERIALIZE(order, (id)(price)(maker)(quantity))
    };

    typedef multi_index<N(orders), order,
            indexed_by<N(byprice), const_mem_fun<order, uint128_t, &order::get_price> >
              > order_index;

    void add_order(order_index* orders, account_name maker, asset quantity, uint128_t price);
};
