#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// TCS230 - Sensor de Cor
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

#define RX_PIN 10
#define TX_PIN 11
#define BUTTON_PIN 13 // Pino do botão

int redMin = 29, redMax = 294;
int greenMin = 27, greenMax = 303;
int blueMin = 23, blueMax = 190;

int redPW = 0, greenPW = 0, bluePW = 0;
int redValue, greenValue, blueValue;

String color1 = ""; // Primeira cor válida
String color2 = ""; // Segunda cor válida
DFRobotDFPlayerMini mp3;
SoftwareSerial softwareSerialMP3(RX_PIN, TX_PIN);

// Declaração do protótipo da função
void playColorSound(String color, bool isMixed = false);

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Configura o botão com resistor pull-up interno

  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  Serial.begin(9600);
  softwareSerialMP3.begin(9600);
  if (!mp3.begin(softwareSerialMP3, true, false)) {
    Serial.println("Erro ao iniciar MP3 Player.");
    while (true);
  }
  mp3.volume(20);
}

void loop() {
  redPW = getRedPW();
  redValue = constrain(map(redPW, redMin, redMax, 255, 0), 0, 255);

  greenPW = getGreenPW();
  greenValue = constrain(map(greenPW, greenMin, greenMax, 255, 0), 0, 255);

  bluePW = getBluePW();
  blueValue = constrain(map(bluePW, blueMin, blueMax, 255, 0), 0, 255);

  String detectedColor = identifyColor(redValue, greenValue, blueValue);

  // Variável estática para armazenar a última cor detectada
  static String lastColor = "";

  // Se a cor detectada for válida e diferente da última cor registrada
  if (detectedColor != "Unknown" && detectedColor != "" && detectedColor != lastColor) {
    lastColor = detectedColor; // Atualiza a última cor detectada
    color2 = color1; // Move a cor anterior para a posição secundária
    color1 = detectedColor; // Atualiza a cor primária com a nova detecção

    Serial.println("Cor detectada: " + detectedColor);
    playColorSound(detectedColor); // Reproduz o som da cor detectada
    delay(5000); // Aguarda o som terminar
  }

  // Verifica se o botão foi pressionado para misturar cores
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(200); // Debounce para evitar leituras falsas

    if (color1 != "" && color2 != "") {
      String mixedColor = mixColors(color1, color2);
      if (mixedColor != "") {
        Serial.println("Cor misturada: " + mixedColor);
        playColorSound(mixedColor, true); // Indica que é mistura
        delay(5000); // Espera o áudio terminar
      } else {
        Serial.println("Mistura não válida.");
        playColorSound("Invalid");
        delay(5000); // Espera o áudio terminar
      }
    } else {
      Serial.println("Cores insuficientes para mistura.");
      playColorSound("Invalid");
      delay(5000); // Espera o áudio terminar
    }
  }

  delay(500); // Pequena espera para estabilização
}


int getRedPW() {
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  return pulseIn(sensorOut, LOW);
}

int getGreenPW() {
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  return pulseIn(sensorOut, LOW);
}

int getBluePW() {
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  return pulseIn(sensorOut, LOW);
}

String identifyColor(int red, int green, int blue) {
  if (red < 15 && green < 15 && blue < 15) return "";

  if (red > 230 && green > 230 && blue > 230) return "White";
  if (red > 235 && green < 170 && blue < 170) return "Red";
  if (red < 250 && green > 170 && blue < 150) return "Green";
  if (red < 120 && green < 195 && blue > 210) return "Blue";
  if (red > 250 && green > 210 && blue < 230) return "Yellow";

  return "Unknown";
}


String mixColors(String color1, String color2) {
  if ((color1 == "Red" && color2 == "Yellow") || (color1 == "Yellow" && color2 == "Red")) {
    return "Orange";
  }
  if ((color1 == "Blue" && color2 == "Yellow") || (color1 == "Yellow" && color2 == "Blue")) {
    return "Green";
  }
  if ((color1 == "Red" && color2 == "Blue") || (color1 == "Blue" && color2 == "Red")) {
    return "Purple";
  }
  return ""; // Sem mistura válida
}

void playColorSound(String color, bool isMixed) {
  if (isMixed) {
    if (color == "Orange") {
      mp3.play(12);
    } else if (color == "Green") {
      mp3.play(14);
    } else if (color == "Purple") {
      mp3.play(13);
    }
  } else {
    if (color == "White") {
      mp3.play(8);
    } else if (color == "Red") {
      mp3.play(3);
    } else if (color == "Green") {
      mp3.play(6);
    } else if (color == "Blue") {
      mp3.play(1);
    } else if (color == "Yellow") {
      mp3.play(2);
    } else if (color == "Orange") {
      mp3.play(4);
    } else if (color == "Black") {
      mp3.play(7);
    } else if (color == "Purple") {
      mp3.play(5);
    } else if (color == "Invalid") {
      mp3.play(11);
    } else {
      mp3.play(9);
    }
  }
delay(200);
}