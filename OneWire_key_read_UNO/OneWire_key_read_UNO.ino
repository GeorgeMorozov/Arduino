#include <OneWire.h>
OneWire iButton(10); // создаём объект 1-wire на 10 выводе
//Key : 1 55 CD 6 1 0 0 94 -- old key
//Key : 1 54 5E 1D 1 0 0 F6  -- new key  


void setup (void) {
  Serial.begin(9600);
}

void loop(void) {
  delay(1000); // задержка 1 сек
  byte addr[8]; // массив для хранения данных ключа
  
  if ( !iButton.search(addr) ) { // если ключ не приложен
      Serial.println("No key connected..."); // сообщаем об этом
      return; // и прерываем программу
  }
  
  Serial.print("Key : ");
  for(int i=0; i<8; i++) {
    Serial.print(addr[i], HEX); // выводим побайтно данные ключа
    Serial.print(" ");
  }
  Serial.println();
  iButton.reset(); // сброс ключа
}
