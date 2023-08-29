#include <stdio.h>
#include <stdlib.h>

typedef struct matrix {
    struct matrix* right;
    struct matrix* below;
    int line;
    int column;
    float info;
} Matrix;

//creates nodes for storing the elements
Matrix* createNode(int lin, int col, float val) {
    Matrix* newNode = (Matrix*)malloc(sizeof(Matrix));
    if (newNode == NULL) {
        printf("error");
        exit;
    }
    newNode->right = NULL;
    newNode->below = NULL;
    newNode->line = lin;
    newNode->column = col;
    newNode->info = val;
    return newNode;
}
//houses the element
void insertElement(Matrix* m, int lin, int col, float val) {
    //creates the node for the new element
    Matrix* newNode = createNode(lin, col, val);

    Matrix* cLine = m;
    //stops at the provided line
    while (cLine->line != lin) {
        cLine = cLine->below;
    }
    //stops at the provided column
    Matrix* cCol = m;
    while (cCol->column != col) {
        cCol = cCol->right;
    }
    //since the element must be stored inside a 2D matrix, 'temp' will be used to move right and down, starting from a different position than the head (m)
    //the element is part of two lists at the same time
    Matrix* temp = cLine;
    while (temp->right != NULL && temp->right->column < col) {
        temp = temp->right;
    }
    newNode->right = temp->right;
    temp->right = newNode;

    temp = cCol;
    while (temp->below != NULL && temp->below->line < lin) {
        temp = temp->below;
    }
    newNode->below = temp->below;
    temp->below = newNode;
}

Matrix* matrix_create() {
    int lines, cols;
    printf("Insert rows: ");
    scanf("%d", &lines);
    printf("Insert columns: ");
    scanf("%d", &cols);

    //creates the header
    Matrix* head = createNode(-1, -1, 0.0);
    Matrix* cLine = head;
    Matrix* cCol = head;
    //creates all the outer '-1' heads
    for (int i = 0; i < lines; ++i) {
        cLine->below = createNode(i, -1, 0.0);
        cLine = cLine->below;
    }

    for (int j = 0; j < cols; ++j) {
        cCol->right = createNode(-1, j, 0.0);
        cCol = cCol->right;
    }

    while (1) {
        int lin, col;
        float val;

        printf("Type the ROW (0 to stop [DO NOT GO OUT OF BOUNDS]): ");
        scanf("%d", &lin);
        if (lin == 0) {
            break;
        }

        printf("Type the COLUMN (0 to stop [DO NOT GO OUT OF BOUNDS]): ");
        scanf("%d", &col);
        if (col == 0) {
            break;
        }

        printf("Type the VALUE: ");
        scanf("%f", &val);

        //properly stores the elements using the provided input (one at the time)
        insertElement(head, lin - 1, col - 1, val); 
    }

    return head;
}

void matrix_print(Matrix* m) {
    //passes through every column for every row
    printf("\n");
    printf("Matrix: \n");

    Matrix* cLin = m->below;
    Matrix* cCol = cLin->right;

    while (cLin != NULL) {
        cCol = cLin->right;
        while (cCol != NULL) {
            //+1 needed to adjust the indexes
            printf("(%d, %d, %.2f) ", cCol->line + 1, cCol->column + 1, cCol->info);
            cCol = cCol->right;
        }
        printf("\n");
        cLin = cLin->below;
    }
}

void matrix_destroy(Matrix* m) {
    //similar to matrix_print but instead of printing is freeing
    Matrix* cLin = m->below;
    Matrix* cCol = cLin->right;

    while (cLin != NULL) {
        cCol = cLin->right;
        while (cCol != NULL) {
            Matrix* temp = cCol; //temp is being used here since cCol itself mustn't be freed in order for the loop to work properly
            cCol = cCol->right;
            free(temp);
        }
        Matrix* temp = cLin; //same here
        cLin = cLin->below;
        free(temp);
    }
    free(m);
}

Matrix* matrix_add(Matrix* m, Matrix* n) {
    // WARNING For this function to work the matrices must have compatible dimension. This function doesn't check that
    printf("\n");
    // A totally new matrix is being created here to store the results
    Matrix* reshead = createNode(-1, -1, 0.0);
    Matrix* rescLin = reshead;

    // These will be used to move down the matrices
    Matrix* LineM = m->below;
    Matrix* LineN = n->below;

    //line by line
    while (LineM != NULL) {
        rescLin->below = createNode(LineM->line, -1, 0.0);
        rescLin = rescLin->below;

        Matrix* rescCol = rescLin;

        Matrix* elementM = LineM->right;
        Matrix* elementN = LineN->right;

        while (elementM != NULL && elementN != NULL) {
            
            float sum = elementM->info + elementN->info;
             if (sum != 0.0) {
                 rescCol->right = createNode(rescLin->line, elementM->column, sum); //if it didn't sum zeros then it stores the result
                 rescCol = rescCol->right;
            }
            //move through the columns
            elementM = elementM->right;
            elementN = elementN->right;
        }
        //moves to next row
        LineM = LineM->below;
        LineN = LineN->below;
    }

    return reshead;
}

Matrix* matrix_multiply(Matrix* m, Matrix* n) {
    //very much the same as matrix_add but now it multiplies
    // WARNING For this function to work the matrices must have compatible dimension. This function doesn't check that
    printf("\n");
    // A totally new matrix is being created here to store the results
    Matrix* reshead = createNode(-1, -1, 0.0);
    Matrix* rescLin = reshead;

    // These will be used to move down the matrices
    Matrix* LineM = m->below;
    Matrix* LineN = n->below;

    //line by line
    while (LineM != NULL) {
        rescLin->below = createNode(LineM->line, -1, 0.0);
        rescLin = rescLin->below;

        Matrix* rescCol = rescLin;

        Matrix* elementM = LineM->right;
        Matrix* elementN = LineN->right;

        while (elementM != NULL && elementN != NULL) {
            float mults = elementM->info * elementN->info;
             if (mults != 0.0) {
                 rescCol->right = createNode(rescLin->line, elementM->column, mults); //if it didn't multiply zeros then it stores the result
                 rescCol = rescCol->right;
            }
            //traverse through the columns
            elementM = elementM->right;
            elementN = elementN->right;
        }
        //go to next row
        LineM = LineM->below;
        LineN = LineN->below;
    }

    return reshead;
}

Matrix* matrix_transpose(Matrix* m) {
    //the idea is similar to all others iterations, but this time the columns are the fixed starting points
    Matrix* reshead = createNode(-1, -1, 0.0);
    Matrix* rescLin = reshead;

    Matrix* cCol = m->right;

    // traverse through the columns of the original matrix  
    while (cCol != NULL) {
        rescLin->below = createNode(cCol->column, -1, 0.0); //inversed line and column
        rescLin = rescLin->below;
        Matrix* rescCol = rescLin;

        // traverse through the elements in the current column
        Matrix* element = cCol->below;

        while (element != NULL) {
            rescCol->right = createNode(element->column, element->line, element->info);//inversed line and column
            rescCol = rescCol->right;
            element = element->below;
        }

        cCol = cCol->right;
    }

    return reshead;
}

//searches for specific element
float matrix_getelem(Matrix* m, int x, int y) {
    x--;
    y--;

    Matrix* cLin = m->below;

    while (cLin != NULL && cLin->line != x) {
        cLin = cLin->below;
    }
    //if row not found
    if (cLin == NULL) {
        return 0.0; 
    }

    Matrix* cCol = m->right;
    while (cCol != NULL && cCol->column != y) {
        cCol = cCol->right;
    }
    //if column not found
    if (cCol == NULL) {
        return 0.0; 
    }

    Matrix* element = cLin->right;
    while (element != NULL && element->column != y) {
        element = element->right;
    }

    if (element == NULL) {
        return 0.0;  // Element not found
    }

    return element->info;
}
//overwrites or inserts a new element
void matrix_setelem(Matrix* m, int x, int y, float elem) {
    x--;
    y--;
    Matrix* cLin = m->below;
    while (cLin != NULL && cLin->line != x) {
        cLin = cLin->below;
    }
    Matrix* element = cLin->right;
    while (element != NULL && element->column != y) {
        element = element->right;
    }

    if (element == NULL) {
        // Element not found, insert a new element
        insertElement(m, x, y, elem);
    } else {
        // Update existing element
        element->info = elem;
    }
}


int main() {
    /*
    Matrix *A = matrix_create();
    matrix_print( A );
    Matrix *B = matrix_create(); 
    matrix_print( B );
    Matrix *C = matrix_add( A, B ); 
    matrix_print( C );
    matrix_destroy( C );
    C = matrix_multiply( A, B ); 
    matrix_print( C );
    matrix_destroy( C );
    C = matrix_transpose( A ); 
    matrix_print( C );
    matrix_destroy( C );
    matrix_destroy( A );
    matrix_destroy( B );
    return 0;
*/  
