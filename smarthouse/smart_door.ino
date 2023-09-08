#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>
Servo myservo;
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
#define Password_Lenght 7 // Give enough room for six chars + NULL char
int pos = 0;    // variable to store the servo position
char Data[Password_Lenght]; // 6 is the number of chars it can hold + the null char = 7
char Master[Password_Lenght] = "123456";
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
bool door = true;
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// LiquidCrystal_I2C lcd(0x21, 16, 2);  
bool creatingPassword = false;
char newPassword[Password_Lenght];


void setup()

{

  myservo.attach(12);

  ServoClose();

  lcd.begin(16, 2);

  lcd.print("Smart House Door");

  lcd.setCursor(0, 1);

  lcd.print("---");

  delay(3000);

  lcd.clear();

}

// ... (previous code remains the same)

void loop()
{
  if (door == 0)
  {
    customKey = customKeypad.getKey();
    if (customKey == '#')
    {
      lcd.clear();
      ServoClose();
      lcd.print("  Door is close");
      delay(3000);
      door = 1;
    }
    else if (customKey == 'A')
    {
      NewPassword();
    }

  }
  else
  {
    customKey = customKeypad.getKey();
    if (customKey == 'A')
    {
      // Start the password creation process
      creatingPassword = true;
      // CreateNewPassword();
    }
    else if (!creatingPassword)
    {
      Open();
    }
  }
}


void clearData()

{

  while (data_count != 0)

  { // This can be used for any array size,

    Data[data_count--] = 0; //clear array for new data

  }

  return;

}

void ServoOpen()

{

  for (pos = 180; pos >= 0; pos -= 5) { // goes from 0 degrees to 180 degrees

    // in steps of 1 degree

    myservo.write(pos);              // tell servo to go to position in variable 'pos'

    delay(15);                       // waits 15ms for the servo to reach the position

  }

}

void ServoClose()

{

  for (pos = 0; pos <= 180; pos += 5) { // goes from 180 degrees to 0 degrees

    myservo.write(pos);              // tell servo to go to position in variable 'pos'

    delay(15);                       // waits 15ms for the servo to reach the position

  }

}

void Open()
{
  lcd.setCursor(0, 0);
  lcd.print(" Enter Password");

  customKey = customKeypad.getKey();
  
  if (customKey == 'A')
  {
    lcd.clear();
    NewPassword(); // Start the password change process
    return; // Exit the Open() function
  }

  // Loop to get the complete password input
  while (data_count < Password_Lenght - 1)
  {
    customKey = customKeypad.getKey();
    
    if (customKey) // A key is pressed
    {
      Data[data_count] = customKey; // Store the character into the data array
      lcd.setCursor(data_count, 1); // Move cursor to show each new character
      lcd.print('*'); // Print an asterisk (*) for each character (masking the password)
      data_count++; // Increment data array by 1 to store the new character
    }
  }

  // Check if the entered password matches the master password
  if (!strcmp(Data, Master)) // If the passwords match
  {
    lcd.clear();
    ServoOpen();
    lcd.print("  Door is Open");
    door = 0;
  }
  else // If the passwords don't match
  {
    lcd.clear();
    lcd.print("  Wrong Password");
    delay(1000);
    door = 1;
  }

  // Clear the data array and reset data_count for the next input
  clearData();
}
void NewPassword()
{
  lcd.setCursor(0, 0);
  lcd.print("Enter Old Password");

  // Loop to get the complete old password input
  while (data_count < Password_Lenght - 1)
  {
    customKey = customKeypad.getKey();

    if (customKey) // A key is pressed
    {
      Data[data_count] = customKey; // Store the character into the data array
      lcd.setCursor(data_count, 1); // Move cursor to show each new character
      lcd.print('*'); // Print an asterisk (*) for each character (masking the password)
      data_count++; // Increment data array by 1 to store the new character
    }
  }

  // Check if the entered old password matches the master password
  if (!strcmp(Data, Master)) // If the passwords match
  {
    lcd.clear();
    lcd.print("Enter New Password");
    int new_data_count = 0;
    int new_Password_Lenght = 7;
    // Loop to get the complete new password input
    while (new_data_count < new_Password_Lenght - 1)
    {
      customKey = customKeypad.getKey();

      if (customKey) // A key is pressed
      {
        newPassword[new_data_count] = customKey; // Store the character into the new password array
        lcd.setCursor(new_data_count, 1); // Move cursor to show each new character
        lcd.print('*'); // Print an asterisk (*) for each character (masking the password)
        new_data_count++; // Increment new_data array by 1 to store the new character

        if (customKey == '#') // Check for '#' to indicate the new password input is complete
        {
          newPassword[new_data_count] = '\0'; // Null-terminate the new password
          break; // Exit the loop
        }
      }
    }

    // Store the new password and update the master password
    strcpy(Master, newPassword);

    lcd.clear();
    lcd.print("New Password Set");
    delay(1000);
    door = 1;
    clearData();
  }
  else // If the passwords don't match
  {
    lcd.clear();
    lcd.print("Wrong Password");

    // You may want to add a delay here to give the user time to read the message
    delay(1000);
    door = 1;
    clearData();
  }
}

    