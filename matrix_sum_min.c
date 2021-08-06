#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

typedef enum {
	STATE_NONE,
	STATE_STAR,
	STATE_PRIME
}
state_t;

typedef struct {
	int64_t cost;
	int64_t cost_bak;
	state_t state;
}
cell_t;

typedef struct {
	int row;
	int col;
}
location_t;

void set_cell(cell_t *, int64_t);
int step1(void);
int step2(void);
int step3(void);
int step4(location_t *);
int find_zero(location_t *);
int step5(location_t *);
int step6(void);
void set_location(location_t *, int, int);
void clear_lines(void);

int order, matrix_size, *rows, *cols;
cell_t *matrix;
location_t *path;

int main(void) {
	int random_flag, step, i;
	int64_t sum;
	location_t start;
	if (scanf("%d%d", &order, &random_flag) != 2 || order < 1) {
		fprintf(stderr, "Invalid settings\n");
		fflush(stderr);
		return EXIT_FAILURE;
	}
	matrix_size = order*order;
	matrix = malloc(sizeof(cell_t)*(size_t)matrix_size);
	if (!matrix) {
		fprintf(stderr, "Could not allocate memory for matrix\n");
		fflush(stderr);
		return EXIT_FAILURE;
	}
	if (random_flag) {
		set_cell(matrix, INT64_C(123456789));
		for (i = 1; i < matrix_size; i++) {
			set_cell(matrix+i, (22695477*matrix[i-1].cost+12345)%1073741824);
		}
	}
	else {
		for (i = 0; i < matrix_size; i++) {
			int64_t cost;
			if (scanf("%"SCNi64, &cost) != 1 || cost < 0) {
				fprintf(stderr, "Invalid cost\n");
				fflush(stderr);
				free(matrix);
				return EXIT_FAILURE;
			}
			set_cell(matrix+i, cost);
		}
	}
	rows = calloc((size_t)order, sizeof(int));
	if (!rows) {
		fprintf(stderr, "Could not allocate memory for rows\n");
		fflush(stderr);
		free(matrix);
		return EXIT_FAILURE;
	}
	cols = calloc((size_t)order, sizeof(int));
	if (!cols) {
		fprintf(stderr, "Could not allocate memory for cols\n");
		fflush(stderr);
		free(rows);
		free(matrix);
		return EXIT_FAILURE;
	}
	path = malloc(sizeof(location_t)*(size_t)matrix_size);
	if (!path) {
		fprintf(stderr, "Could not allocate memory for path\n");
		fflush(stderr);
		free(cols);
		free(rows);
		free(matrix);
		return EXIT_FAILURE;
	}
	step = 1;
	while (step != 7) {
		switch (step) {
			case 1:
				step = step1();
				break;
			case 2:
				step = step2();
				break;
			case 3:
				step = step3();
				break;
			case 4:
				step = step4(&start);
				break;
			case 5:
				step = step5(&start);
				break;
			case 6:
				step = step6();
				break;
			default:
				break;
		}
	}
	sum = 0;
	printf("Assignment");
	for (i = 0; i < order; i++) {
		int j;
		for (j = 0; j < order; j++) {
			if (matrix[i*order+j].state == STATE_STAR) {
				sum += matrix[i*order+j].cost_bak;
				printf(" %d", j);
			}
		}
	}
	printf("\nSum %"PRIi64"\n", sum);
	fflush(stdout);
	free(path);
	free(cols);
	free(rows);
	free(matrix);
	return EXIT_SUCCESS;
}

void set_cell(cell_t *cell, int64_t cost) {
	cell->cost = cost;
	cell->cost_bak = cost;
	cell->state = STATE_NONE;
}

int step1(void) {
	int i;
	for (i = 0; i < order; i++) {
		int j;
		int64_t cost_min = matrix[i*order].cost;
		for (j = 1; j < order; j++) {
			if (matrix[i*order+j].cost < cost_min) {
				cost_min = matrix[i*order+j].cost;
			}
		}
		for (j = 0; j < order; j++) {
			matrix[i*order+j].cost -= cost_min;
		}
	}
	return 2;
}

int step2(void) {
	int i;
	for (i = 0; i < order; i++) {
		int j;
		for (j = 0; j < order; j++) {
			if (matrix[i*order+j].cost == 0 && !rows[i] && !cols[j]) {
				matrix[i*order+j].state = STATE_STAR;
				rows[i] = 1;
				cols[j] = 1;
			}
		}
	}
	clear_lines();
	return 3;
}

int step3(void) {
	int covered = 0, i;
	for (i = 0; i < order; i++) {
		int j;
		for (j = 0; j < order; j++) {
			if (matrix[i*order+j].state == STATE_STAR) {
				cols[j] = 1;
			}
		}
	}
	for (i = 0; i < order; i++) {
		if (cols[i]) {
			covered++;
		}
	}
	if (covered == order) {
		return 7;
	}
	return 4;
}

int step4(location_t *start) {
	while (1) {
		int col;
		if (!find_zero(start)) {
			return 6;
		}
		matrix[start->row*order+start->col].state = STATE_PRIME;
		for (col = 0; col < order && matrix[start->row*order+col].state != STATE_STAR; col++);
		if (col == order) {
			return 5;
		}
		rows[start->row] = 1;
		cols[col] = 0;
	}
}

int find_zero(location_t *zero) {
	int i;
	for (i = 0; i < order; i++) {
		int j;
		for (j = 0; j < order; j++) {
			if (matrix[i*order+j].cost == 0 && !rows[i] && !cols[j]) {
				set_location(zero, i, j);
				return 1;
			}
		}
	}
	set_location(zero, order, order);
	return 0;
}

int step5(location_t *start) {
	int path_idx = 0, i;
	set_location(path, start->row, start->col);
	while (1) {
		int row, col;
		for (row = 0; row < order && matrix[row*order+path[path_idx].col].state != STATE_STAR; row++);
		if (row == order) {
			break;
		}
		path_idx++;
		set_location(path+path_idx, row, path[path_idx-1].col);
		for (col = 0; col < order && matrix[path[path_idx].row*order+col].state != STATE_PRIME; col++);
		path_idx++;
		set_location(path+path_idx, path[path_idx-1].row, col);
	}
	for (i = 0; i <= path_idx; i++) {
		if (matrix[path[i].row*order+path[i].col].state == STATE_STAR) {
			matrix[path[i].row*order+path[i].col].state = STATE_NONE;
		}
		else if (matrix[path[i].row*order+path[i].col].state == STATE_PRIME) {
			matrix[path[i].row*order+path[i].col].state = STATE_STAR;
		}
	}
	for (i = 0; i < matrix_size; i++) {
		if (matrix[i].state == STATE_PRIME) {
			matrix[i].state = STATE_NONE;
		}
	}
	clear_lines();
	return 3;
}

int step6(void) {
	int i;
	int64_t cost_min = INT64_MAX;
	for (i = 0; i < order; i++) {
		int j;
		for (j = 0; j < order; j++) {
			if (matrix[i*order+j].cost < cost_min && !rows[i] && !cols[j]) {
				cost_min = matrix[i*order+j].cost;
			}
		}
	}
	for (i = 0; i < order; i++) {
		int j;
		for (j = 0; j < order; j++) {
			if (rows[i]) {
				matrix[i*order+j].cost += cost_min;
			}
			if (!cols[j]) {
				matrix[i*order+j].cost -= cost_min;
			}
		}
	}
	return 4;
}

void set_location(location_t *location, int row, int col) {
	location->row = row;
	location->col = col;
}

void clear_lines(void) {
	int i;
	for (i = 0; i < order; i++) {
		rows[i] = 0;
	}
	for (i = 0; i < order; i++) {
		cols[i] = 0;
	}
}
