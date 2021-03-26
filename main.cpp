#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

//VARIABLES GLOBALES

    int rc=0;
    unsigned int num_escritores, num_lectores;

//SEMAFOROS

    sem_t mutex,db;

//FUNCIONES CONSTRUCTURAS

    void *lector(void *p);
    void *escritor(void *p);
    void escribirenbasedatos();
    void leerenbasedatos();

int main(){

    int estado;
    int aux=1;
    cout << "LEONARDO ARANDA GUTIERREZ 217761528" << endl;
    cout << "ESTE PROGRAMA CORRESPONDE A 1 ESCRITOR Y N LECTORES" << endl;
    cout << "CENTRO UNIVERSITARIO DE CIENCIAS EXACTAS E INGENIERIAS" << endl;
    cout << "INGENIERIA INFORMATICA" << endl;
    cout << "SISTEMAS OPERATIVOS." << endl <<endl;
    cout << "Ingrese el numero de lectores para comenzar..." << endl;
    cin >> aux;


    //CREAMOS Y CERRAMOS EL ARCHIVO-RECURSO
    ofstream recurso("recurso.txt", ios::app);
    recurso.close();

    //DETERMINAMOS EL NUMERO DE LECTORES Y ESCRITORES
    num_escritores = 1;
    num_lectores = aux;

    int eid[num_escritores],lid[num_lectores];

    //DECLARAMOS LOS HILOS

    pthread_t escritora[num_escritores],lectora[num_lectores];

    //INICIALIZAMOS LOS SEMÁFOROS

    sem_init(&mutex,0,1);
    sem_init(&db,0,1);


    //ASIGNACIÓN DE HILOS A CADA ESCRITOR Y LECTOR
    for(unsigned int i=0;i<num_escritores;i++){
        eid[i]=i;
        if((estado=pthread_create(&escritora[i],NULL,escritor,(void *) &eid[i])))
            exit(estado);
    }

    for(unsigned int i=0;i<num_lectores;i++){
        lid[i]=i;
        if((estado=pthread_create(&lectora[i],NULL,lector,(void *) &lid[i])))
            exit(estado);

    }


    //ENTRADA DE HILOS DE FORMA SIMULTANEA

    for(unsigned int i=0;i<num_escritores;i++){
        pthread_join(escritora[i],NULL);
    }


    for(unsigned int i=0;i<num_lectores;i++){
        pthread_join(lectora[i],NULL);
    }

    //DESTRUCCIÓN DE SEMÁFOROS

    sem_destroy(&mutex);
    sem_destroy(&db);

    //SE REMUEVE EL ARCHIVO
    remove("recurso.txt");

    return 0;
}

void escribirenarchivo(){//EN ESTA FUNCION EL ESCRITOR ESCRIBE SOBRE EL RECURSO

    ofstream recurso("recurso.txt", ios::app);
    recurso << "Hola" << endl;
    recurso.close();

}

void leerenarchivo(){

    ifstream recurso("recurso.txt", ios::in);
    if (recurso.is_open()) {
        while (!recurso.eof()) {
            string temp;

            if (recurso.eof()) {
                break;
            }

            getline(recurso, temp);

            if (temp == "Escritura realizada") {
                cout << "Se detecto: " << temp << endl;
            }

            if (recurso.eof()) {
                break;
            }
        }
    }
    recurso.close();

}

void *escritor(void *p){

        extern sem_t mutex,db;

        int *id_es;
            id_es=(int *)p;


        sem_wait(&db);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
        printf("\nEscritor %d Escribiendo...\n",*id_es);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        escribirenarchivo();
        sem_post(&db);


        pthread_exit(NULL);
}

void *lector(void *p){

        extern sem_t mutex,db;
        extern int rc;
        int *id_le;

            id_le=(int *)p;

        sem_wait(&mutex);
        rc++;
        if(rc == 1)
        sem_wait(&db);

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        printf("\nLector %d Leyendo...\n",*id_le);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        leerenarchivo();
        sem_post(&mutex);
        sem_wait(&mutex);
        rc--;

        if(rc == 0)
          sem_post(&db);

        sem_post(&mutex);
        pthread_exit(NULL);
}
