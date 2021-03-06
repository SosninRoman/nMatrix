#include <iostream>
#include "nMatrix.h"

int main(int argc, char* argv[])
{
	size_t size[3] = {2,3,3};
	nMatrix<int, 3> matrix(size);

	matrix[0][0][0] = 2;  
	matrix[0][0][1] = 5;  
	matrix[0][0][2] = 4;  

	matrix[0][1][0] = 3;
	matrix[0][1][1] = 8;
	matrix[0][1][2] = 12;

	matrix[0][2][0] = 1;  
	matrix[0][2][1] = 4;  
	matrix[0][2][2] = 5;  

	matrix[1][0][0] = 2;  
	matrix[1][0][1] = 2;  
	matrix[1][0][2] = 7;  

	matrix[1][1][0] = 8;
	matrix[1][1][1] = 11;
	matrix[1][1][2] = 3;

	matrix[1][2][0] = 1;  
	matrix[1][2][1] = 2;  
	matrix[1][2][2] = 5; 

	auto c = matrix[0][2][0];
	/*std::cout << '\n';
	std::cout << ".................. \n";

	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			for(int k = 0; k < 3; ++k)
			{
				int value = matrix[i][j][k];
				std::cout << value << " ";
			}
			std::cout << '\n';
		}
		std::cout << '\n';
		std::cout << "................... \n";
	}*/
	//

	nMatrix<int, 3> matrix_copy_matrix(matrix);

	matrix_copy_matrix[0] += 600;
	//std::cout << '\n' << matrix_copy_matrix.size() << " " << matrix_copy_matrix.get<0>() << " " << matrix_copy_matrix.get<1>() << " " << matrix_copy_matrix.get<2>();
	//std::cout << '\n';
	//
	nMatrix<double, 3> matrix_double_copy_matrix(matrix);

	matrix_copy_matrix += 5; 

	//
	nMatrix<double,2> matrix_from_slice(matrix[1]);

	//
	nMatrix<int, 3> matrix_operation_eq(size);
	matrix_operation_eq = matrix;

	//
	size_t small_size[3] = {1,1,1};
	nMatrix<int, 3> small_matrix(small_size);
	nMatrix<int, 3> matrix_operation_eq2(matrix);
	matrix_operation_eq2 = small_matrix;

	//
	nMatrix<int, 2> matrix_operation_eq_slice(size);
	matrix_operation_eq_slice = matrix[0];

	//
	nMatrix<int, 1> one_dim_matrix(3);
	one_dim_matrix[0] = 2;
	one_dim_matrix[1] = 7;
	one_dim_matrix[2] = 4;
	nMatrix<int, 1> const_matrix(one_dim_matrix);

	one_dim_matrix[1] = 444;
	//
	const nMatrix<int, 3> const_matrix_copy_matrix(matrix);
	int vvv = const_matrix_copy_matrix[0][0][0];
	nMatrix<int,2> two_dim_matrix(const_matrix_copy_matrix[1]);

	//
	size_t d2[2] = {2,2};
	nMatrix<int, 2> square_matrix(d2);
	square_matrix[0][0] = 2;
	square_matrix[0][1] = 5;
	square_matrix[1][0] = 3;
	square_matrix[1][1] = 8;
	nMatrix<int, 2> square_matrix2(d2);
	square_matrix2[0][0] = 1;
	square_matrix2[0][1] = 1;
	square_matrix2[1][0] = 1;
	square_matrix2[1][1] = 1;

	square_matrix += 100;

	square_matrix -= 50;

	square_matrix[0] += 600;

	return 0;
}