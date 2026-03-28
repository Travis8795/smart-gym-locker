# smart-gym-locker
This is a locker system that uses RFID and demonstrates how its logic works with a servo, LEDs, and a speaker for feedback.

## How it works
The gate will be open by default and has no owner, and once a card logs in, it saves this card ID as its owner and gets locked to it, and it can't be opened with any other owner, and once you log in again, it unlocks and removes the saved card (owner).

Here is a [video](https://youtube.com/shorts/OfdslvYBTxI?feature=share) of it working.
## How to make
Order the parts in the BOM section and wire it like the following picture, then flash the code from the source files using [Arduino IDE](https://www.arduino.cc/en/software/).

![Smart Locer schema](https://github.com/user-attachments/assets/7005b0cf-7938-4a0c-8a91-e0ac272bacab)

## BOM

| Part       |
| ----------------- |
| Arduino uno   |
|  buzzer or small toy speaker  |
| RC522 RFID |
| servo |
| different color LED * 2 |
| 9V or 12V battery |
