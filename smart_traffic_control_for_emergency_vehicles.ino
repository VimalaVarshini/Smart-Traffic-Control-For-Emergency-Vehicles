#define enA 10     // Enable1 L298 Pin enA
#define in1 9      // Motor1 L298 Pin in1
#define in2 8      // Motor1 L298 Pin in2
#define in3 7      // Motor2 L298 Pin in3
#define in4 6      // Motor2 L298 Pin in4
#define enB 5      // Enable2 L298 Pin enB
#define ir_R s1    // IR sensor Right
#define ir_F s2    // IR sensor Front
#define ir_L s3    // IR sensor Left
#define servoPin 3 // Servo control pin
#define pump 4     // Pump control pin

int Speed = 160; // Motor speed (0 to 255)
int s1, s2, s3;  // Sensor readings

void setup() {
    Serial.begin(9600); // Start serial communication at 9600bps

    // Initialize IR sensor pins
    pinMode(ir_R, INPUT);
    pinMode(ir_F, INPUT);
    pinMode(ir_L, INPUT);

    // Initialize motor driver pins
    pinMode(enA, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    pinMode(enB, OUTPUT);

    // Initialize servo and pump pins
    pinMode(servoPin, OUTPUT);
    pinMode(pump, OUTPUT);

    // Set motor speed
    analogWrite(enA, Speed);
    analogWrite(enB, Speed);

    // Initialize servo position
    initializeServo();
}

void loop() {
    // Read sensor values
    s1 = analogRead(ir_R);
    s2 = analogRead(ir_F);
    s3 = analogRead(ir_L);

    // Print sensor values for debugging
    Serial.print("s1: ");
    Serial.print(s1);
    Serial.print("\ts2: ");
    Serial.print(s2);
    Serial.print("\ts3: ");
    Serial.println(s3);
    
    delay(50); // Small delay to stabilize sensor readings

    // Control logic based on sensor values
    if (s1 < 250) {
        Stop();
        digitalWrite(pump, HIGH);
        moveServo(90, 40, 3);
    } else if (s2 < 350) {
        Stop();
        digitalWrite(pump, HIGH);
        moveServo(90, 140, 3);
        moveServo(140, 40, 3);
        moveServo(40, 90, 3);
    } else if (s3 < 250) {
        Stop();
        digitalWrite(pump, HIGH);
        moveServo(90, 140, 3);
        moveServo(140, 90, 3);
    } else if (s1 >= 251 && s1 <= 700) {
        digitalWrite(pump, LOW);
        backword();
        delay(100);
        turnRight();
        delay(200);
    } else if (s2 >= 251 && s2 <= 800) {
        digitalWrite(pump, LOW);
        forword();
    } else if (s3 >= 251 && s3 <= 700) {
        digitalWrite(pump, LOW);
        backword();
        delay(100);
        turnLeft();
        delay(200);
    } else {
        digitalWrite(pump, LOW);
        Stop();
    }
    
    delay(10); // Small delay to prevent rapid changes
}

void initializeServo() {
    // Move servo to different angles to initialize
    for (int angle = 90; angle <= 140; angle += 5) {
        servoPulse(servoPin, angle);
    }
    for (int angle = 140; angle >= 40; angle -= 5) {
        servoPulse(servoPin, angle);
    }
    for (int angle = 40; angle <= 95; angle += 5) {
        servoPulse(servoPin, angle);
    }
    delay(500); // Wait for servo to reach the position
}

void moveServo(int startAngle, int endAngle, int step) {
    if (startAngle < endAngle) {
        for (int angle = startAngle; angle <= endAngle; angle += step) {
            servoPulse(servoPin, angle);
        }
    } else {
        for (int angle = startAngle; angle >= endAngle; angle -= step) {
            servoPulse(servoPin, angle);
        }
    }
}

void servoPulse(int pin, int angle) {
    int pwm = map(angle, 0, 180, 544, 2400); // Convert angle to microseconds
    digitalWrite(pin, HIGH);
    delayMicroseconds(pwm);
    digitalWrite(pin, LOW);
    delay(20); // Servo refresh rate
}

void forword() { // Move forward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
}

void backword() { // Move backward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
}

void turnRight() { // Turn right
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
}

void turnLeft() { // Turn left
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
}

void Stop() { // Stop motors
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
}
