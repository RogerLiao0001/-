# README

## 介紹
這是一個視覺小說遊戲的引擎。

## 要如何執行這個程式
請在執行 `make` 後，輸入 `./engine` 來執行程式。

## GitHub 連結
[https://github.com/RogerLiao0001/Programming-project](https://github.com/RogerLiao0001/Programming-project)

## 功能實裝

### 基本功能
1. 支援場景、人物、物品等實體
2. 支援場景背景、人物頭像、人物立繪、物品圖標等圖片素材
3. 支援場景背景與人物立繪之疊加顯示
4. 可放置物品的背包
   - 背包內會顯示玩家資訊和道具
5. 支援角色個人心情數值
6. 支援對話選項與多支線劇情設定
   - 使用者可以透過選擇物品或是對話改變結局
7. 好感度數值系統
   - 利用選項可以提高或降低好感度，或是偵測好感度(大於或小於)顯示隱藏選項，好感度可以是各種數值(血量等等)

### 額外功能
8. 遊戲存檔
   - 可以儲存和載入進度，且遊戲狀態存檔為人類可讀，且可直接使用文字編輯器更改。
9. 自動存檔
   - 在點擊回到標題、離開遊戲、右上角 x 和進入新對話時會自動存檔在第一個存檔的位置
10. 場景動畫
    - 場景變換時，會有一秒以上的漸入漸出的特效。
11. 心情變換
    - 在不同時機可以替換角色的頭像與立繪。
12. 音效播放
    - 可以在開頭畫面、事件中放入不同背景音樂，可以在劇本中放入插入音效，或是劇本語音配音。
13. 用 LLM 生成劇情
    - 可生成三個角色、三個場景、兩個物品、三個結局
14. 更改預設圖示
    - 可以在 default.toml 更改遊戲內圖示，下面有更改教學。
15. 道具介紹
    - 點選道具時會顯示道具的介紹
16. 隱藏對話框功能
    - 可以隱藏對話框和對話，欣賞後面的角色或美術圖
17. 設定功能
    - 可以調節音量大小、返回遊戲、回到標題和離開遊戲
18. 道具系統
    - 利用選項可以獲得道具，或是偵測背包內是否有所需道具顯示隱藏選項
19. 角色介紹
    - 點擊角色大頭貼會顯示角色介紹

## 基本遊戲設定
在 `assets` 裡面的 `default.toml` 可以改變遊戲預設的東西。

```toml
game_name="請輸入標題"
```
在兩個雙引號中，可以改變遊戲上方標題。

### 對話框
```toml
chatbar="assets/image/default/chatbar.png"
```
可以更改對話框圖示，前面是連到圖檔的目錄

### 背包、儲存、設定、隱藏對話、載入、關閉
以背包為例：
```toml
backpack_bg="assets/image/default/bg.jpg"
backpack_1="assets/image/default/backpack_1.png"
backpack_2="assets/image/default/backpack_2.png"
```
- bg 可以更改界面後面的的背景
- 1 是一基本圖標
- 2 是鼠標放在上面是會改變的圖標

### 基本按鈕、選項按鈕、背包按鈕、儲存和載入按鈕
以選項按鈕為例
```toml
options_button_1="assets/image/default/optionbar1.png"
options_button_2="assets/image/default/optionbar2.png"
```
- 1 是一基本圖標 
- 2 是鼠標放在上面是會改變的圖標

### 遺失的圖片
```toml
missing_image="assets/image/default/missing.png"
```
如果劇本檔設定未正確連接到遊戲內的圖案，會顯示該圖示。

### 按按鈕音效
```toml
button_sound="assets/sound/default/button.wav"
```
按下按鈕會撥放的音效

## 劇本檔格式
請把劇本檔命名為 `script.toml` 再依據以下格式，創作劇本檔。請標籤皆使用英文、數字和 "_"

### 玩家
```toml
[player]
(必要)
name=" "
avatar="檔案位置/檔案名稱"
(不必要)
describe=" "
inventory=["道具標籤"," "," "] (不一定要有)
```

### 角色
```toml
[charactar.角色標籤]
(必要)
name=" "
(不必要)
describe=" "
like= (如果不加預設是0)
大頭貼：
avatar_表情標籤="檔案位置/檔案名稱"
站姿：
tachie_表情標籤="檔案位置/檔案名稱"
```
(同一個表情一定要有 avatar 和 tachie)

### 範例
#### 物品
```toml
[item.道具標籤]
name=" "
description=" "
icon="檔案位置/檔案名稱"
```

#### 場景
```toml
[scene.場景標籤]
name=" "
background="檔案位置/檔案名稱"
```

#### 事件
第一個事件一定要設為 `event.start`
```toml
[event.事件標籤]
(必要)
name=" "
scene="場景標籤"
dialogue="對話標籤"
(不必要)
music="檔案位置/檔案名稱"
```

#### 對話
結尾一定要是選項、next、event、end。
```toml
[dialogue.對話標籤]
(必要)
text[號碼]=" "
(放字幕 後面號碼從1開始往後增加)
下一個對話或事件或結束或選項(擇一)
next="對話標籤"
(抓下一段對話 就切換到[dialogue.對話標籤])
event="事件標籤"
(抓下一個事件 就切換到[event.事件標籤])
end=" "
(如果要結束就加 end 後面是寫結局)
(不必要)
(要先有 character 才可以有 emote)
character[號碼]="角色標籤"
(抓角色 後面號碼從1開始往後增加)
emote[號碼]="表情標籤"
emote[號碼]=" clear" (可用來清除表情)
(放表情 後面號碼從1開始往後增加)
sound[號碼]="檔案位置/檔案名稱"
(放音效 後面號碼從1開始往後增加)
nowtalk[號碼]=" "
(放現在說話是誰 後面號碼從1開始往後增加)
(如果要清掉，雙引號內甚麼都不要加)
```

#### 選項
如果要選項，請加在該對話後面，對話中不可有 event 或 next，不然會先連接到其他對話或事件！
```toml
[dialogue.對話標籤.options.號碼]
(必要)
文字
text=" "
下一個對話或事件(擇一)
next="對話標籤"
event="事件標籤"
(不必要)
like=["角色標籤",要增加數值(可正負)]
(改變好感度)
隱藏選項(偵測好感度)
hide_like_bigger=["角色標籤",數值] (如果大於就顯示)
hide_like_smaller=["角色標籤",數值] (如果小於就顯示)
隱藏選項(偵測道具)
hide_item="道具標籤"
(如果有這個道具就顯示)
刪除道具
remove_item="道具標籤"
獲得道具
get_item="道具標籤"
```

## LLM 自動生成劇本、圖片

### 介紹
使用 OpenAI API 生成使用者選定之主題

的 toml 劇本，並且以 DALL-E 生成對應之圖片，並用 Microsoft 提供之去背 API 進行角色圖片去背。

### 使用方式
1. 進入 llm 資料夾
2. 在 main.c 前兩行填入自己的 OpenAI API key
3. 使用 make 指令
4. 輸入劇本主題
5. 等待大約五分鐘(依網路速度而定)
6. 將生成好的劇本、去背好的圖片丟到遊戲引擎中

### 注意事項
1. 如果劇本檔有誤，可能會造成遊戲崩潰！
2. 如果有抓到 bug，歡迎利用以下信箱聯絡引擎開發人員 41171216H@gapps.ntnu.edu.tw
