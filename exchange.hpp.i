/**
 * simple decentralized exchange
 * smart contract on eos
 *
 */

#pragma once

#include <string>

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/multi_index.hpp>


using namespace eosio;

namespace simple_decentralized_exchange {
  class exchange : public contract {
    public:
      using contract::contract;

      //@abi action
      void testaddbid(account_name maker, asset quantity, uint128_t price);

      /*
      //@abi action
      void bid(account_name maker, asset quantity, uint128_t price);
      //@abi action
      void ask(account_name maker, asset quantity, uint128_t price);

      //@abi action
      void cancel_bid(account_name maker, uint64_t id);
      //@abi action
      void cancel_ask(account_name maker, uint64_t id);
      */

    private:
      //@abi table
      struct order {
        uint64_t id;

        uint128_t price;
        asset quantity;
        account_name maker;

        uint64_t primary_key() const { return id; }
        uint128_t by_price() const { return price; }
      };

      typedef multi_index<N(order), order,
              indexed_by<N(byprice),
                const_mem_fun<order, uint128_t, &order::by_price> >
              > order_index;

      typedef std::iterator<std::bidirectional_iterator_tag, const order> iterator;

      /*
      void deposit(account_name maker, asset quantity);
      void withdraw(account_name maker, asset quantity);
      */
      void add_order(order_index orders, account_name maker, asset quantity, uint128_t price);
      //void sub_order(order_index orders, iterator& it, asset quantity, account_name payer);
  };
}
