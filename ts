watch -n 1 'echo eosio && cleos get currency balance eosio.token eosio'
watch -n 1 'echo alice && cleos get currency balance eosio.token alice'

watch -n 1 'echo bids && cleos get table exchange 4292493394837504000 orders'

watch -n 1 'echo asks && cleos get table exchange 3900117277302849536 orders'

cleos set account permission exchange active '{"threshold":"1","keys":[{"key":"EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight":"1"}],"accounts":[{"permission":{"actor":"exchange","permission":"eosio.code"},"weight":"1"}]}' owner -p exchange
