/**
 * simple decentralized exchange
 * smart contract on eos
 *
 */

#include <exchange.hpp>

using namespace eosio;

namespace simple_decentralized_exchange {
  void exchange::deposit(account_name from, account_name contract, asset quantity) {

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

EOSIO_ABI(simple_decentralized_exchange::exchange, (deposit)(withdrawal)(bid)(ask))
