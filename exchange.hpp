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

      void bid(account_name maker, extended_asset quantity, int64_t price);
      void ask(account_name maker, extended_asset quantity, int64_t price);

      void cancel_bid(account_name maker, int64_t id);
      void calcel_ask(account_name maker, int64_t id);

      void add_currency(account_name contract);
      void remove_currency(account_name contract);

    private:
      struct order {
        int64_t id;
        account_name maker;
        asset quantity;
        int64_t price;
        int64_t primary_key()const { return id; }
      };

      struct currency {
        account_name contract;
        int64_t primary_key()const {return contract;}
      }

      typedef multi_index<N(bids), order> bids;
      typedef multi_index<N(asks), order> asks;

      typedef multi_index<N(currencies), currency> currencies;
  }
}
