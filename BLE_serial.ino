#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

#define READBUFFERSIZE (32)

SoftwareSerial g_bluetooth(6, 7);
LiquidCrystal g_lcd( 12, 11, 5, 4, 3, 2 );
char g_szReadBuffer[READBUFFERSIZE] = "";
int  g_iIndexChar = 0;
int g_iCounter = 0;  // カウンター

void setup()
{
  // ボーレートを指定して通信開始
  g_bluetooth.begin(9600);
  
  g_lcd.begin( 16, 2 );
  g_lcd.clear();
  g_lcd.print("(ready)");
}

// １行文字列の読み込み
// 0 : 読み取り途中。1 : 読み取り完了。
int ReadLineString( SoftwareSerial& serial,
          char szReadBuffer[], const int ciReadBufferSize, int& riIndexChar,
          char szLineString[], const int ciLineStringSize )
{
  while( 1 )
  {
    char c = serial.read();
    if( -1 == c )
    {
      break;
    }
    if( '\r' == c  )
    { // 終端
      szReadBuffer[riIndexChar] = '\0';
      strncpy( szLineString, szReadBuffer, ciLineStringSize - 1 );
      szLineString[ciLineStringSize - 1] = '\0';
      riIndexChar = 0;
      return 1;
    }
    else if( '\n' == c )
    {
      ; // 何もしない
    }
    else
    { // 途中
      if( (ciReadBufferSize - 1) > riIndexChar )
      {
        szReadBuffer[riIndexChar] = c;
        riIndexChar++;
      }
    }
  }

  return 0;
}

void loop()
{
  // データ送信
  g_bluetooth.println(g_iCounter);
  Serial.println();
  g_iCounter++;
  delay( 100 );

  // データ読み取り
  char szLineString[READBUFFERSIZE];
  if( !ReadLineString( g_bluetooth,
             g_szReadBuffer, READBUFFERSIZE, g_iIndexChar,
             szLineString, READBUFFERSIZE ) )
  { // 読み取り途中
    return;
  }
  // 読み取り完了
  g_lcd.clear();
  g_lcd.setCursor( 0, 0 );
  g_lcd.print(szLineString);
}

