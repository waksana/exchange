/**
 * simple decentralized exchange
 * smart contract on eos
 *
 */

#pragma once

#include <string>

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>

using namespace eosio;

namespace simple_decentralized_exchange {
  class exchange : public contract {
    public:
      exchange(account_name self):contract(self){}

      void deposit(account_name from, asset quantity);
      void withdrawal(account_name to, asset quantity);

      void bid(asset quantity, int64_t price);
      void ask(asset quantity, int64_t price);
    private:
      struct account {
        asset balance;
        asset on_order;
        uint64_t primary_key()const { return balance.symbol.name(); }
      };
      struct order {
        asset amount;
        int64_t price;
        account_name maker;
        uint64_t primary_key()const { return amount.symbol.name(); }
      };

      typedef multi_index<N(accounts), account> accounts;
      typedef multi_index<N(bids), order> bids;
      typedef multi_index<N(asks), order> asks;

     void add_balance( account_name owner, asset value, account_name ram_payer );
     void sub_balance( account_name owner, asset value );
  }
}
