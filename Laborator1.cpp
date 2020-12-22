#include<iostream>
#include<mpi.h>
using namespace std;
struct mType {
	int value;
	int cols;
	int rows;
};
// Траспанирование матрицы
int * reverseArray(int *arr,int numbCols,int numbRows){
	int **arr_2D = new int*[numbRows];
	for(int i = 0; i < numbRows;i++){
		arr_2D[i] = new int [numbCols];
	}
	int k = 0;
	for(int i = 0; i < numbRows; i++){
		for(int j = 0; j < numbCols; j++,k++){
			arr_2D[i][j] = arr[k];
		}
	}
	k = 0;
	int * arr_get = new int [numbCols*numbRows];
	for(int i = 0;i < numbRows;i++){
		for(int j = 0;j < numbCols;j++,k++){
			arr_get[k] = arr_2D[j][i];
		}
	}
	return arr_get;
}
// Вычисление максимумов из ряда элементов по определенному расстоянию
int* getMaxValuesArr(int * arr, int numbElements,int numbCols){
	int elements = numbElements/numbCols;
	int numbSuspect = numbCols-1;
	int *maxElements = new int [elements];
	int max = arr[0];
	int k = 0;
	for(int i = 0; i < numbElements; i++){
		if(max < arr[i]){
			max = arr[i];
		}
		if(i%numbCols-numbSuspect == 0){
			maxElements[k] = max; 
			max = arr[i+1];
			k++;
		}
	}
	return maxElements;
}
// Разбиение матрицы на равное кол.во элементов для каждого процессора 
int * BrokeArray(int *arr,int numbRows,int numbCols,int numbProc){
	if(numbProc > numbRows){
		numbProc = numbRows;
	}
	int flag = 0;
	if(numbProc == numbRows-2) flag = 1;
	int *arr_sizing = new int [numbProc];
	for(int i = 0;i < numbProc; i++){
		arr_sizing[i] = 1;
	}
	int numbRowsCopy = numbRows;
	while(true){
		numbRowsCopy -= numbProc;
		if(numbRowsCopy == 0){
			for(int i = 0; i < numbProc; i++){
				arr_sizing[i]++;
			}
			break;
		}
		if(numbRowsCopy < 0){
			arr_sizing[0]++;
			if(flag == 1) {
				arr_sizing[1]++;
			}
			break;
		}

		for(int i = 0; i < numbProc; i++){
			arr_sizing[i]++;
		}
		
	}

	// minus one for all and get number elements
	for(int i = 0; i < numbProc;i++){
		arr_sizing[i]--;
		arr_sizing[i]*=numbCols;
	}
	return arr_sizing; 
}
int *sortMaxBooble(int *max,int numberElements){
	for(int i = 0; i < numberElements;i++){
		for(int j = 0; j < numberElements-1-i;j++){
			if(max[j] < max[j+1]) {
				int temp = max[j];
				max[j] = max[j+1];
				max[j+1] = temp;
			}
		}
	}

	return max;
}
mType* getCountMaxElements(int *matrix,int numbRows , int numbCols,int *max){
	int **matrix2D = new int*[numbRows];
	for(int i = 0; i < numbRows; i++) {
		matrix2D[i] = new int[numbCols];
	}
	int count = 0;
	for(int i = 0; i < numbRows; i++ ) {
		for(int j = 0; j < numbCols; j++ , count++ ) {
			matrix2D[i][j] = matrix[count];
		}
	}
	count = 0;
	max = sortMaxBooble(max,numbRows);
	// show1DArray(max,numbRows);
	mType *maxElements = new mType[numbRows*numbCols];
	for(int i = 0; i < numbRows; i++) {
		int flagFound = 0;
		for(int j = 0; j < numbRows; j++) {
			int flagExist = 0;
			if(flagFound == 1){
				break;
			}
			// add check if in max Elements exist element with same rows if yes continue
			if(count != 0) {
				for(int s = 0; s < count; s++) {
					if(maxElements[s].rows == j) {
						flagExist = 1;
						break;
					}
				}
			}
			if(flagExist != 1) {
				for(int k = 0; k < numbCols; k++) {
					if(max[i] == matrix2D[j][k]) {
						maxElements[count].value = max[i];
						maxElements[count].rows = j;
						maxElements[count].cols = k;
						count++;
						flagFound = 1;
					}
				}
			}

		}
	}
	// return right array
	mType *returnArrays = new mType[count+1];
	for(int i = 0; i < count;i++) {
		returnArrays[i] = maxElements[i];
		if(i == count - 1) {
			returnArrays[count].value = 0;
			returnArrays[count].rows = numbRows+1;
			returnArrays[count].cols = numbCols+1;
		}
	}

	return returnArrays;
}
// Высчитывает равновесие Нэша с выводом на экран результата
void showNash(int *matrixA,int *matrixB,int numberRows,int numberCols,int *maxA,int *maxB) {

	mType *matrixAS = getCountMaxElements(matrixA,numberRows,numberCols,maxA);
	mType *matrixBS = getCountMaxElements(matrixB,numberRows,numberCols,maxB);
	
	// Высчитываем количество найденых элементов с индексом из матрицы А
	int countA = 0;
	while (true)
	{
		countA++;
		if(matrixAS[countA].rows == numberRows+1) {
			break;
		}
	}

	// Высчитываем количество найденных элементов  с Индексом из матрицы B
	int countB = 0;
	while (true)
	{
		countB++;
		if(matrixBS[countB].rows == numberRows+1) {
			break;
		}
	}
	int k = 0;
	for(int i = 0; i < countA;i++) {
		for(int j = 0;j < countB;j++) {
				if( matrixAS[i].cols == matrixBS[j].rows && matrixAS[i].rows == matrixBS[j].cols) {
					cout << "Nash in: " << endl;
		 			cout << " [ " <<  matrixAS[i].rows << " " <<  matrixAS[i].cols << " ] " << matrixAS[i].value << " [ " << matrixBS[j].cols << " " << matrixBS[j].rows << " ] " << matrixBS[j].value << endl; 
					k++;
			}
		}
		
	}
		
	if(k == 0){
	   cout << "This matrix isn`t nash "<< endl;
	}
}

int main(int argc,char *argv[]) {

	MPI_Init(NULL,NULL);
	int my_rank,size_ranks;
	// Необходимо менять на размер каждой вычисляемой матрицы
	int numbRows = 6;
	int numbCols = 6;
	
	MPI_Comm_size( MPI_COMM_WORLD , &size_ranks);
	MPI_Comm_rank( MPI_COMM_WORLD , &my_rank);
	if(size_ranks > numbRows){
		size_ranks = numbRows;
	}
	int buf_size = numbCols*numbRows;
	int *buf = new int[buf_size];
	int *buf1 = new int[buf_size];
	int *arrayElement = new int[size_ranks];
	int *arrayElement1 = new int [size_ranks];
	
	if(my_rank == 0){

		int *matrixA = new int [numbRows*numbCols];
		int *matrixB = new int [numbRows*numbCols];
		
		int *bufferAGet = new int [numbRows];
		int *bufferBGet = new int [numbRows];
		int countFullA = 0;
		int countFullB = 0;
		// Пример 6.2
		// matrixA[0] = 2; matrixA[1] = 0; matrixA[2] = 1; 
     	// matrixA[3] = 3; matrixA[4] = 2; matrixA[5] = 0; 
		// matrixA[6] = 0; matrixA[7] = 1; matrixA[8] = 2;

		// matrixB[0] = 1; matrixB[1] = 0; matrixB[2] = 2;
     	// matrixB[3] = 2; matrixB[4] = 1; matrixB[5] = 0; 
		// matrixB[6] = 0; matrixB[7] = 2; matrixB[8] = 1;

		// Пример 6.1 С переворотом Матрицы Б по условию
		// matrixA[0] = 400; matrixA[1] = 0; matrixA[2] = 0; matrixA[3] = 0; matrixA[4] = 0; matrixA[5] = 0; 
     	// matrixA[6] = 300; matrixA[7] = 300; matrixA[8] = 0; matrixA[9] = 0; matrixA[10] = 0; matrixA[11] = 0; 
		// matrixA[12] = 200 ; matrixA[13] = 200; matrixA[14] = 200; matrixA[15] = 0; matrixA[16] = 0; matrixA[17] = 0; 
		// matrixA[18] = 100; matrixA[19] = 100; matrixA[20] = 100; matrixA[21] = 100; matrixA[22] = 0; matrixA[23] = 0; 
     	// matrixA[24] = 0; matrixA[25] = 0; matrixA[26] = 0; matrixA[27] = 0; matrixA[28] = 0; matrixA[29] = 0; 
		// matrixA[30] = -100; matrixA[31] = -100; matrixA[32] = -100; matrixA[33] = -100; matrixA[34] = -100; matrixA[35] = -100; 

		// matrixB[0] = 0; matrixB[1] = 200; matrixB[2] = 100; matrixB[3] = 0; matrixB[4] = -100; matrixB[5] = -200;
     	// matrixB[6] = 0; matrixB[7] = 0; matrixB[8] = 100; matrixB[9] = 0; matrixB[10] = -100; matrixB[11] = -200;
		// matrixB[12] = 0; matrixB[13] = 0; matrixB[14] = 0; matrixB[15] = 0; matrixB[16] = -100; matrixB[17] = -200;
		// matrixB[18] = 0; matrixB[19] = 0; matrixB[20] = 0; matrixB[21] = 0; matrixB[22] = -100; matrixB[23] = -200;
     	// matrixB[24] = 0; matrixB[25] = 0; matrixB[26] = 0; matrixB[27] = 0; matrixB[28] = 0; matrixB[29] = -200;
		// matrixB[30] = 0; matrixB[31] = 0; matrixB[32] = 0; matrixB[33] = 0; matrixB[34] = 0; matrixB[35] = 0;

		// Пример 6.1 С переворотом Матрицы А 
		matrixB[0] = 400; matrixB[1] = 0; matrixB[2] = 0; matrixB[3] = 0; matrixB[4] = 0; matrixB[5] = 0; 
     	matrixB[6] = 300; matrixB[7] = 300; matrixB[8] = 0; matrixB[9] = 0; matrixB[10] = 0; matrixB[11] = 0; 
		matrixB[12] = 200 ; matrixB[13] = 200; matrixB[14] = 200; matrixB[15] = 0; matrixB[16] = 0; matrixB[17] = 0; 
		matrixB[18] = 100; matrixB[19] = 100; matrixB[20] = 100; matrixB[21] = 100; matrixB[22] = 0; matrixB[23] = 0; 
     	matrixB[24] = 0; matrixB[25] = 0; matrixB[26] = 0; matrixB[27] = 0; matrixB[28] = 0; matrixB[29] = 0; 
		matrixB[30] = -100; matrixB[31] = -100; matrixB[32] = -100; matrixB[33] = -100; matrixB[34] = -100; matrixB[35] = -100; 

		matrixA[0] = 0; matrixA[1] = 200; matrixA[2] = 100; matrixA[3] = 0; matrixA[4] = -100; matrixA[5] = -200;
     	matrixA[6] = 0; matrixA[7] = 0; matrixA[8] = 100; matrixA[9] = 0; matrixA[10] = -100; matrixA[11] = -200;
		matrixA[12] = 0; matrixA[13] = 0; matrixA[14] = 0; matrixA[15] = 0; matrixA[16] = -100; matrixA[17] = -200;
		matrixA[18] = 0; matrixA[19] = 0; matrixA[20] = 0; matrixA[21] = 0; matrixA[22] = -100; matrixA[23] = -200;
     	matrixA[24] = 0; matrixA[25] = 0; matrixA[26] = 0; matrixA[27] = 0; matrixA[28] = 0; matrixA[29] = -200;
		matrixB[30] = 0; matrixA[31] = 0; matrixA[32] = 0; matrixA[33] = 0; matrixA[34] = 0; matrixA[35] = 0;

		// Транспанируем матрицу с сохранением ее адресса
		matrixB = reverseArray(matrixB,numbCols,numbRows);
		
		// Разбиваем матрицу А на равный количество элементов для каждого процессора 
		int *arrDelay = BrokeArray(matrixA,numbRows,numbCols,size_ranks);
		// Разбиваем матрицу B на равный количество элементов для каждого процессора 
		int *arrDelay1 = BrokeArray(matrixB,numbRows,numbCols,size_ranks); 

        // Присваивание разбитую матрицу А на кол.во элементов в глобальном на уровне main
			for(int i = 0; i < size_ranks; i++){
				arrayElement[i] = arrDelay[i]; 
			}
		// Присваивание разбитую матрицу B на кол.во элементов в глобальном на уровне main
			for(int i = 0;i < size_ranks;i++){
				arrayElement1[i] = arrDelay1[i];
			}

        // Находим промежуток через который будет передавать данные из однорядной матрицы А
             int *displacements = new int [size_ranks + 1];
			 displacements[0] = 0;
			 int numb = 0;
			 for(int i = 0,k = 1;i < size_ranks + 1; i++ , k++){
				 numb += arrDelay[i];
				 displacements[k] = numb;
			 };
			 int *displacements1 = new int [size_ranks + 1];
			 displacements1[0] = 0;
			 int numb1 = 0;
			 for(int i = 0,k = 1;i < size_ranks + 1; i++ , k++){
				 numb1 += arrDelay1[i];
				 displacements1[k] = numb1;
			 };
		// Отправляем данные из матрицы А всем процессорам
		MPI_Bcast(&*arrayElement, size_ranks, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Scatterv(matrixA, arrayElement, displacements, MPI_INT, &*buf, buf_size, MPI_INT, 0, MPI_COMM_WORLD);
		// Высчитываем максимумы из матрицы А которые были переданны процессору 0
		 int number = arrayElement[my_rank];
		 int *maxElements = getMaxValuesArr(buf,number,numbCols);
		 int numberMaxElements = number/numbCols;
		
		 for(int i = 0; i < numberMaxElements;i++,countFullA++){
			bufferAGet[countFullA] = maxElements[i];
		  }
		int *get_elements1 = new int [numberMaxElements];
		
		// Считывание максимумов полученных с каждого процессора матрицы А
		for(int i = 1; i < size_ranks; i++){
		    MPI_Status status;
            MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            int count;
            MPI_Get_count(&status, MPI_INT, &count);
			int *get_elements1 = new int[count];
			MPI_Recv(&*get_elements1, count , MPI_INT , MPI_ANY_SOURCE , MPI_ANY_TAG , MPI_COMM_WORLD , MPI_STATUS_IGNORE);
				for(int i = 0; i < count;i++,countFullA++){
					bufferAGet[countFullA] = get_elements1[i];
				}
		}
		// Отправляем данные из матрицы А всем процессорам
		MPI_Bcast(&*arrayElement1, size_ranks, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Scatterv(matrixB, arrayElement1, displacements1, MPI_INT, &*buf1, buf_size, MPI_INT, 0, MPI_COMM_WORLD);

		// Высчитываем максимумы из матрицы А которые были переданны процессору 0
		int number1 = arrayElement1[my_rank];
	
		int *maxElements1 = getMaxValuesArr(buf1,number1,numbCols);
		int numberMaxElements1 = number1/numbCols;
	
		for(int i = 0; i < numberMaxElements1;i++,countFullB++){
			bufferBGet[countFullB] = maxElements1[i];
		}

		// Считывание максимумов полученных с каждого процессора матрицы А
		int *get_elements2 = new int [numberMaxElements1];
   		for(int i = 1; i < size_ranks; i++) {
			MPI_Status status1;
            MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status1);
            int count1;
            MPI_Get_count(&status1, MPI_INT, &count1);
			int *get_elements2 = new int[count1];
			MPI_Recv(&*get_elements2, count1 , MPI_INT , MPI_ANY_SOURCE , MPI_ANY_TAG , MPI_COMM_WORLD , MPI_STATUS_IGNORE);
			for(int i = 0; i < count1;i++,countFullB++){
				bufferBGet[countFullB] = get_elements2[i];
			}
		}

		// Присылаем основные матрицы а так же найденные максимумы на проверку 
		showNash(matrixA,matrixB,numbRows,numbCols,bufferAGet,bufferBGet);

	} else {
		
	if(my_rank <= numbRows && my_rank != 0) {
		// Считывание данных для определение необходимых элементов из матрицы А
		MPI_Bcast(&*arrayElement, size_ranks, MPI_INT, 0, MPI_COMM_WORLD);
		int number = arrayElement[my_rank];
		MPI_Scatterv(NULL, NULL, NULL, MPI_INT, &*buf, buf_size, MPI_INT, 0, MPI_COMM_WORLD);
		// Высчитывание максимум из каждой строки присланных из процессора 0 матрицы А
		int *maxElements = getMaxValuesArr(buf,number,numbCols);
		int numberMaxElements = number/numbCols;
		MPI_Send(&*maxElements , numberMaxElements , MPI_INT , 0 , 0 , MPI_COMM_WORLD);
		// Считывание данных для определение необходимых элементов Б
		MPI_Bcast(&*arrayElement1, size_ranks, MPI_INT, 0, MPI_COMM_WORLD);
		int number1 = arrayElement1[my_rank];
		MPI_Scatterv(NULL, NULL, NULL, MPI_INT, &*buf1, buf_size, MPI_INT, 0, MPI_COMM_WORLD);
		// Высчитывание максимум из каждой строки присланных из процессора 0 матрицы А
	    int *maxElements1 = getMaxValuesArr(buf1,number1,numbCols);
		int numberMaxElements1 = number1/numbCols;
		MPI_Send(&*maxElements1 , numberMaxElements1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD);


	}
	
}
	
	MPI_Finalize();
	return 0;
}