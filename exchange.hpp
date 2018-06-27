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

      void deposit(asset quantity);
      void withdrawal(asset quantity);

      void bid(asset quantity, 
  }
}
