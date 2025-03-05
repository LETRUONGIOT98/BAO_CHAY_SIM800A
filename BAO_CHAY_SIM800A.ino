
#include <Adafruit_MLX90614.h> // thư viện cảm biến đo nhiệt độ
String val;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
int coi = 13, bom = 2, gas = A0, lua = 4, i = 0,k=0, j =0, m =0, dien =3, den =12; //Set chân cho các biến
#include<SoftwareSerial.h>
SoftwareSerial sim(11, 10); //RX, TX  cài đặt chân nhận và truyền dữ liệu với module sim
void setup() {  //hàm cài đặt IO
  Serial.begin(9600);  //Mở cổng serial giao tiếp với máy tính tốc độ là 9600
  sim.begin(9600); //Mở cổng serial giao tiếp với module sim tốc độ là 9600
  delay(100); 
  //Các lệnh kiểm tra sim
  sim.println("AT");
  sim.println("AT+CMGF=1");
  sim.println("AT+CNMI=1,2,0,0,0");
  delay(500);
 //Set IO cho các biến đc gán
  pinMode(coi, OUTPUT);
  pinMode(bom, OUTPUT);
  pinMode(den, OUTPUT);
  pinMode(dien, OUTPUT);
  pinMode (lua, INPUT_PULLUP);
  digitalWrite(dien, LOW);
  lcd.init();  //khởi động màn hình
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("   SMART HOME   "); //hiển thị lên màn hình chữ smart home


  if (!mlx.begin()) {  //nếu khởi dộng cảm biến lỗi thì gửi lỗi lên máy tính
    Serial.println("Error connecting to MLX sensor. Check wiring.");
  }

  
  
}

void loop() {   //Chương trình chính
  if(sim.available()>0){  //Nếu có dữ liệu từ sim gửi về thì
    val = sim.readStringUntil('\n');  //lưu dữ liệu đó vào biến val
    
    Serial.println(val);}
  int t = mlx.readObjectTempC();  //Gán dữ liệu nhận đc từ cảm biến nhiệt độ vào biến t
  int k = map(analogRead(gas), 0, 1023, 0, 100); // gán giá trị từ module gas và biến k
  
  delay(10);
  lcd.setCursor(0, 1);
  lcd.print("T:    *C");
  lcd.setCursor(3, 1);
  lcd.print(t);
  lcd.setCursor(9, 1);
  lcd.print("G:    %");
  lcd.setCursor(12, 1);
  lcd.print(k);
  if ((t > 60 && digitalRead(lua) == 1)  ||( t > 60 && k < 20)) { // nếu biên t > 60 và tín hiệu từ chân digital đọc được là mức cao thfi hực hiện lệnh bên dưới
  lcd.setCursor(0, 1);
    lcd.print("NHIET DO QUA CAO");
    digitalWrite(dien, 1);
    digitalWrite(den, 1);
    digitalWrite(coi, HIGH);
    digitalWrite(bom, 1);
    delay(300);
    if (j < 5) {
      j = j + 1;
    }
    if (j > 5) {
      j = 3;
    }
    if (j < 2) {
      phanhoi(1);    //Gửi tin nhắn về sim
    }
    digitalWrite(coi, LOW);
    lcd.setCursor(0, 1);
  lcd.print("                ");
    delay(300);
  }
  if( t< 60){
    j = 0;
    
  }
  
  if ((k >= 20 && digitalRead(lua) == 1) || (k >= 20 && t < 60)) {
  lcd.setCursor(0, 1);
    lcd.print("PHAT HIEN KHI GA");
    digitalWrite(coi, HIGH);
    digitalWrite(den, 1);
    delay(300);
  if (i < 7) {
      i = i + 1;
    }
    if (i > 7) {
      i = 5;
    }
    if (i < 2) {
      phanhoi(2);
    }  
    digitalWrite(coi, LOW);
    digitalWrite(den, 0);
    lcd.setCursor(0, 1);
  lcd.print("                ");
    delay(300);
  }
  if (k < 20) {
    i=0;
  }
  else{digitalWrite(coi, LOW);

  }
  if ((digitalRead(lua) == 0 && t < 60) || (digitalRead(lua) == 0 && k < 20)) {
    lcd.setCursor(0, 1);
    lcd.print(" PHAT HIEN LUA   ");
    digitalWrite(den, 1);
    digitalWrite(coi, HIGH);
    delay(300);
    if (m < 5) {
     m = m + 1;
    }
    if (m > 5) {
      m = 3;
    }
    if (m < 2) {
    phanhoi(3);}
    digitalWrite(coi, LOW);
    digitalWrite(den, 0);
    lcd.setCursor(0, 1);
  lcd.print("                ");
    delay(300);
  }
  if(digitalRead(lua) == 1) {
    m=0;
  }
  if (digitalRead(lua) == 0 || (k > 20 && digitalRead(lua) == 0)) {
    lcd.setCursor(0, 1);
    lcd.print("   NGUY HIEM:   ");
    if (k < 9) {
      k = k + 1;
    }
    if (k > 9) {
      k = 7;
    }
    if (k < 5) {
      phanhoi(4);
    }
    digitalWrite(dien, 1);
    digitalWrite(den, 1);
    delay(100);
    digitalWrite(coi, HIGH);
    digitalWrite(bom, 1);
    lcd.setCursor(0, 1);
  lcd.print("                ");
  }

  else {
    digitalWrite(coi, 0);
    digitalWrite(bom, 0);
    digitalWrite(dien, 0);
    k=0;
  }
}
void phanhoi(int tipe) {   //Hàm con khi có các lệnh bên trên thực hiện thì hàm này đc gọi lên để gửi tin nhắn về điện thoại
  sim.println("AT+CMGF=1");
  delay(1000);
  sim.println("AT+CMGS=\"0392920568\"\r");   //Số điện thoại nhận tin nhắn cảnh báo
  delay(1000);
 if (tipe == 1) {
    sim.println("CANH BAO NHIET DO QUA CAO");
    Serial.println("CANH BAO NHIET DO QUA CAO");

  }
  if (tipe == 2) {
    sim.println("CANH BAO CO GAS");
    Serial.println("CANH BAO CO GAS");
  }
  if (tipe == 3) {
    sim.println("PHAT HIEN CO TIA LUA");
    Serial.println("PHAT HIEN CO TIA LUA");

  }
  if (tipe == 4) {
    sim.println("CANH BAO NGUY HIEN: CO CHAY");
    Serial.println("CANH BAO NGUY HIEN: CO CHAY");

  } 
  delay(100);
  sim.println((char)26);
  delay(1000);
}
