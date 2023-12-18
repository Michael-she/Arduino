
int dispPin = 6;

int counter = 0;

int shiftRegisterCount = 36;

void setup() {
  // put your setup code here, to run once:

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(dispPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:


  //writeToShiftRegister("11101101");

  encodeInteger(counter + 0.5);
}


void writeToShiftRegister(String seq) {

  for (int i = 0; i < shiftRegisterCount; i++) {

    digitalWrite(2, HIGH);

    digitalWrite(3, HIGH);

    digitalWrite(3, LOW);

    digitalWrite(2, LOW);
  }

  for (int i = 0; i < seq.length(); i++) {

    if (seq.charAt(i) == '1') {
      digitalWrite(2, LOW);
      Serial.print("1");
    } else {
      Serial.print("0");
      digitalWrite(2, HIGH);
    }

    digitalWrite(3, HIGH);

    digitalWrite(3, LOW);

    digitalWrite(2, LOW);
  }
  Serial.println();
  digitalWrite(dispPin, HIGH);

  digitalWrite(dispPin, LOW);
}





String encodeInteger(float inNum) {

  String numberString = String(inNum) + "   ";
  Serial.println(numberString);
  String binString = "";


  int decimalOffset = 0;
  bool decimal = false;
  for (int i = 0; i < 3; i++) {

    if (numberString.charAt(i + 1) == '.') {
      decimal = true;
    }

    switch (numberString.charAt(i + decimalOffset)) {
      case '0':
        if (!decimal) {
          binString += "11101101";
        } else {
          binString += "11111101";
        }
        break;
      case '1':

        if (!decimal) {
          binString += "01100000";
        } else {
          binString += "01110000";
        }
        break;

      case '2':
        if (!decimal) {
          binString += "11001110";
        } else {
          binString += "11011110";
        }
        break;
      case '3':
        if (!decimal) {
          binString += "11101010";
        } else {
          binString += "11111010";
        }
        break;

      case '4':
        if (!decimal) {
          binString += "01100011";
        } else {
          binString += "01110011";
        }
        break;
      case '5':
        if (!decimal) {
          binString += "10101011";
        } else {
          binString += "10111011";
        }
        break;
      case '6':
        if (!decimal) {
          binString += "10101111";
        } else {
          binString += "10111111";
        }
        break;
      case '7':
        if (!decimal) {
          binString += "11100000";
        } else {
          binString += "11110000";
        }
        break;
      case '8':
        if (!decimal) {
          binString += "11101111";
        } else {
          binString += "11111111";
        }
        break;
      case '9':
        if (!decimal) {
          binString += "11100011";
        } else {
          binString += "11110011";
        }
        break;
      default:
        binString += "";
        break;
    }
    if (numberString.charAt(i + 1) == '.') {
      decimal = false;
      decimalOffset = 1;
    }
  }


  writeToShiftRegister(binString);
  Serial.println(binString);
  return binString;
}