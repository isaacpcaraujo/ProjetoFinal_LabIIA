// Import LIBs
//#include <PID_v1.h>


// DEFINIÇÕES DE PINOS
#define dirFrente 3
#define dirTras   5

#define esqFrente 6
#define esqTras   9

// Ultrasonic sensor
#define trig 10
#define echo 11
float distancia;

// Speed
int delayGiro = 600;
int velocidadeE = 70 * 1.16; 
int velocidadeD = 70;

// PID
double Setpoint, Input, Output;
double Kp=0.003, Ki=0.00001, Kd=0.001;
//PID speedPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// Posiçao
double xDistancia = 0;
double yDistancia = 0;
double tempoParaAlcancarX = 10 *1000;
double tempoParaAlcancarY = 10 *1000;

double tempoMovendoY = 0;
double tempoMovendoX = 0;
double tempoDesvioObstaculo = 0;

bool enable;
int state = 0;

void setup() {
    pinMode (trig, OUTPUT);
    pinMode (echo,INPUT);

    pinMode(dirFrente, OUTPUT);
    pinMode(dirTras,   OUTPUT);
    pinMode(esqFrente, OUTPUT);
    pinMode(esqTras,   OUTPUT);

    digitalWrite(dirFrente, LOW);
    digitalWrite(dirTras,   LOW);
    digitalWrite(esqFrente, LOW);
    digitalWrite(esqTras,   LOW);

    distancia = 1000;
    enable = false;

//    speedPID.SetMode(AUTOMATIC);

    Serial.begin(9600);
}

void loop(void)
{
    while (Serial.available() < 1) {
        distanceSensor();

        if (state == 1 && millis() - tempoMovendoY >= tempoParaAlcancarY){ // Entra aqui quando alcançar a posição final Y
            state = 2;
            moveEsq();
            delay(delayGiro);
            tempoMovendoX = millis();
            moveFrente();
        }
        if (state == 2 && millis() - tempoMovendoX >= tempoParaAlcancarX){ // // Entra aqui quando alcançar a posição final X
            state = 0;
            parar();
        }

        if (distancia <= 30 && distancia > 0 && enable && false){
            tempoDesvioObstaculo = millis();
            parar();
            delay(200);

            moveEsq();
            delay(delayGiro);

            moveFrente();
            delay(1000);

            moveDir();          
            delay(delayGiro);

            moveFrente();
            
            if(state == 1){
              tempoParaAlcancarY += tempoDesvioObstaculo - millis();
            }
            if(state == 2){
              tempoParaAlcancarX += tempoDesvioObstaculo - millis();
            }

        }
    }

//    Input = analogRead(PIN_INPUT);
//    speedPID.Compute();
//    analogWrite(PIN_OUTPUT, Output);

    char val = Serial.read();
    direcao(val);
}

void distanceSensor() {
    digitalWrite(trig, LOW);
    delay(0005);
    digitalWrite(trig, HIGH);
    delay(0010);
    digitalWrite(trig, LOW);
    
    distancia = pulseIn (echo, HIGH);
    distancia = distancia/58;

    Serial.print ("distancia: ");
    Serial.println (distancia);
}


void moveFrente(){
    analogWrite(esqFrente, velocidadeE);
    digitalWrite(esqTras,     LOW);
    analogWrite(dirFrente, velocidadeD);
    digitalWrite(dirTras,     LOW);
}

void moveTras(){
    digitalWrite(esqFrente,   LOW);
    analogWrite(esqTras,     velocidadeE);
    digitalWrite(dirFrente,   LOW);
    analogWrite(dirTras,     velocidadeD);
}

void moveDir(){
    analogWrite(esqFrente,   velocidadeE);
    digitalWrite(esqTras,     LOW);
    digitalWrite(dirFrente,   LOW);
    analogWrite(dirTras,     LOW);
}

void moveEsq(){  
    digitalWrite(esqFrente,   LOW);
    analogWrite(esqTras,     velocidadeE);
    analogWrite(dirFrente,   LOW);
    digitalWrite(dirTras,     LOW);
}

void parar (){
    digitalWrite(esqFrente,   LOW);
    digitalWrite(esqTras,     LOW);
    digitalWrite(dirFrente,   LOW);
    digitalWrite(dirTras,     LOW);
}

void addVelocidade(int valor){
    velocidadeE += valor; 
    velocidadeD += valor;
}

void subVelocidade(int valor){
  velocidadeE -= valor; 
  velocidadeD -= valor;
}

void direcao(char val){
    switch(val) 
    {
        case 'w':
            enable = true;
            state = 1;
            tempoMovendoY = millis();
            moveFrente ();
            break;
        case 's':
            moveTras ();
            break;
        case 'a':
            moveEsq ();
            break;
        case 'd':
            moveDir();
            break;
        case 'x':
            enable = false;
            parar ();
            break;
        case '+':
            addVelocidade(10);
            break;
        case '-':
            subVelocidade(10);
            break;
        default:
            break;
    }
}