# Philosophy
MyWallet is an opinionated investment tracker built for my own investment needs. Its goal is to remain simple to use and is meant for aspiring investors who follow my simple investment style (detailed below). I wanted to avoid creating a huge Excel sheet and preferred having a small app to manage everything. This software is still a work in progress and prone to bugs; therefore, it shouldn't be treated as a professional investment tool.

# Is it meant for you ?
MyWallet was made in order to help me keep track of my investment strategy, so it is naturally adequate for it and inadequate for other strategies. Here is a small description of how I invest, in hopes of helping you decide if this software is worth trying:
- I invest regularly (around once per month).
- I invest in multiple assets.
- The assets I invest in are all ETFs.
- My strategy is long-term investment without really selling any assets.  
The features of MyWallet revolve around these following points, so if you do not see your strategy in mine, this tool might not be meant for you. However, if your investment strategy does align with mine, then MyWallet should be worth a try. There is also a roadmap written below to show what I plan to work on, which could include features you are looking for.

# Installation 
- Arch Linux Hyprland setup  
Prerquisits: CMake, clang++ 22.1.8

```bash
git@github.com:Maxime-Leguevacques/MyWallet.git
cd MyWallet
cmake -B build
cmake --build build
```

# Usage
Once installed and built using CMake, go to MyWallet/build and run ./MyWallet
This will launch the application.
As of right now, you can use the 2 tabs: dashboard and assets.
Before adding any orders, you need to first create an asset. Navigate to the asset section and click on the 'new asset' button. Fill in each section and press the 'add' button. Press 'S' to save.
Once an asset is added, you can create an order for it by going back to the dashboard section. Click on the 'new order' button and fill in each section. When done, press the 'add' button. Your order will now appear. Press 'S' to save.

Press 'H' to toggle the help menu.

# ROADMAP
- [ ] better and safer save system
- [ ] include stocks (it technically can already work with stocks by treating them as an ETF but there are a few adjustments to do especially concerning security so it can be very prone to bugs)
- [ ] asset stats (historical positions, profit and other data)
- [ ] portfolio insights
- [ ] assets modification
- [ ] order modification
- [ ] cross platform portability (Windows, Mac and check other Linux distros)
- [ ] easier installation system
- [ ] manage more gracefully selling orders (you also technically can right now by entering negative amounts but I haven't tested it so it's prone to bugs)
