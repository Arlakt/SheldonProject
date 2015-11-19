#define NBECHANTILLON 512
#define NBCAPTEURS  8
#define OFFSET  337

long valeurs[NBCAPTEURS];
long moyennes[NBCAPTEURS]={0,0,0,0,0,0,0,0};
int moyenneSurUnOctet[NBCAPTEURS];
int i=0;
int compteur=0;

bool start = false;


void setup() {
  Serial.begin(9600);
  delay(5000);
}

void loop() {
  for (compteur=0;compteur<NBECHANTILLON;compteur++){
    for (i=0;i<NBCAPTEURS;i++){
      valeurs[i]=analogRead(i) - OFFSET;
      valeurs[i] *= valeurs[i];
      moyennes[i]=(moyennes[i]*compteur+valeurs[i])/(compteur+1);
    }
  }
  for(i=0;i<NBCAPTEURS;i++){
    
    moyenneSurUnOctet[i] = moyennes[i]*255/60000;
    Serial.write(moyenneSurUnOctet[i]);
    // For debug only
    //Serial.print(moyenneSurUnOctet[i]);
    //Serial.print("\n");
    moyennes[i]=0;
  }
  //Serial.print("\n");
}

