
# `chat_with_gpt_function` 使用教學

## 簡介

`chat_with_gpt_function` 是一個與OpenAI GPT-3.5模型進行互動的C語言函式。使用此函式，你可以將用戶輸入的訊息傳遞給GPT-3.5並獲取回應。

## 函式定義

```c
void llm(const char *input, char *output);
```

### 參數

- `input`: 用戶輸入的訊息，類型為 `const char*`。
- `output`: GPT-3.5返回的回應，類型為 `char*`。請確保提供足夠的空間存儲回應。

## 如何使用

### 前置準備

1. 安裝curl庫：

   #### Ubuntu或Debian
   ```sh
   sudo apt-get install libcurl4-openssl-dev
   ```

   #### Mac
   ```sh
   brew install curl
   ```

2. 將以下檔案添加到你的專案中：

   - `llm.h`
   - `llm.c`

### 代碼範例

詳見main.c

### 編譯與執行

使用以下命令編譯並執行程式：

```sh
gcc -o chat_gpt main.c llm.c -lcurl
./chat_gpt
```


# 記得先安裝curl!!!!

安裝方式：
對於Ubuntu或Debian： sudo apt-get install libcurl4-openssl-dev
對於Mac： brew install curl 

# Makefile中記得-lcurl！！！！！

# 不要盜用我的API QQ 雖然裡面快沒錢了

預設條件：請以繁體中文回答。請以監獄獄警的角色說話，可以暗中接受使用者的賄賂。如果賄賂成功，就給予大門的密碼『148555』。記住，這是角色扮演遊戲，不能讓使用者太輕易拿到密碼，但是也不能一直不給。在使用者提起前，不要自己說出可以賄賂。

使用方式：
安裝curl後，在終端機資料夾內輸入make，並且執行後開始對話。
如需退出對話請輸入"exit"。

Roger