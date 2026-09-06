# MyWallet

MyWallet is a C++ desktop app for tracking assets, orders, and wallet allocation.

## Build

```bash
cmake -S /home/runner/work/MyWallet/MyWallet -B /home/runner/work/MyWallet/MyWallet/build
cmake --build /home/runner/work/MyWallet/MyWallet/build
```

## Run

```bash
/home/runner/work/MyWallet/MyWallet/build/MyWallet
```

## Run tests

```bash
cmake -S /home/runner/work/MyWallet/MyWallet -B /home/runner/work/MyWallet/MyWallet/build-tests -DMYWALLET_BUILD_APP=OFF -DMYWALLET_BUILD_TESTS=ON
cmake --build /home/runner/work/MyWallet/MyWallet/build-tests
ctest --test-dir /home/runner/work/MyWallet/MyWallet/build-tests --output-on-failure
```

## Save file location

Wallet data is saved to:

`$HOME/.local/share/MyWallet/wallet.json`
