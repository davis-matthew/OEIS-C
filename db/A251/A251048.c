/*
 * A251048  Number of (n+1) X (1+1) 0..3 arrays with no 2 X 2 subblock having
 *          the sum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 21, u64 n <= 47, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A251048
    Formula:
      Let rows be encoded by 4‑bit numbers where the high 2 bits are the
      first column and the low 2 bits the second column of a row.
      Define the 16×16 adjacency matrix M by
          M[u][v] = 1  if   a + d <= |b - c|
                 = 0  otherwise
      where [a,b] = row u and [c,d] = row v.
      Then a(n) = Σ_{u,v} (M^n)[u][v]  (n ≥ 1).
      Initial values are a(1)=66, a(2)=290, …
*/

#include <stddef.h>

typedef struct {
    seq_t m[16][16];
} Matrix;

static int initialized = 0;          /* flag for one‑time adjacency setup */
static Matrix M;                      /* adjacency matrix        */

/* Build adjacency matrix M[u][v] == 1 iff a+d <= |b - c|. */
static void init_matrix(void)
{
    if (initialized)
        return;
    for (idx_t u = 0; u < 16; ++u)
    {
        idx_t a = u >> 2;          /* first column of row u   */
        idx_t b = u & 3;           /* second column of row u  */
        for (idx_t v = 0; v < 16; ++v)
        {
            idx_t c = v >> 2;      /* first column of row v   */
            idx_t d = v & 3;      /* second column of row v  */
            idx_t diff = (b > c) ? (b - c) : (c - b);
            M.m[u][v] = (a + d <= diff) ? (seq_t)1 : (seq_t)0;
        }
    }
    initialized = 1;
}

/* Zero‑initialize a Matrix. */
static void zero_matrix(Matrix *R)
{
    for (idx_t i = 0; i < 16; ++i)
        for (idx_t j = 0; j < 16; ++j)
            R->m[i][j] = (seq_t)0;
}

/* Multiply A and B, result in C. */
static void multiply(const Matrix *A, const Matrix *B, Matrix *C)
{
    zero_matrix(C);
    for (idx_t i = 0; i < 16; ++i)
    {
        for (idx_t k = 0; k < 16; ++k)
        {
            seq_t aik = A->m[i][k];
            if (aik == (seq_t)0)
                continue;
            for (idx_t j = 0; j < 16; ++j)
            {
                C->m[i][j] += aik * B->m[k][j];
            }
        }
    }
}

/* Compute base^exp (exp as unsigned integer) using binary exponentiation. */
static void matrix_power(const Matrix *base, idx_t exp, Matrix *result)
{
    Matrix out, cur;
    /* Identity matrix */
    zero_matrix(result);
    for (idx_t i = 0; i < 16; ++i)
        result->m[i][i] = (seq_t)1;

    cur = *base;
    while (exp > 0)
    {
        if (exp & (idx_t)1)
        {
            multiply(result, &cur, &out);
            *result = out;
        }
        multiply(&cur, &cur, &out);
        cur = out;
        exp >>= 1;
    }
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* offset 1; n==0 is undefined */

    init_matrix();              /* ensure M is ready */

    Matrix P;
    matrix_power(&M, n, &P);    /* P = M^n */

    /* Sum all entries of P.  This equals the number of walks of length n
       in the graph, i.e. the number of valid (n+1)×2 arrays. */
    seq_t total = (seq_t)0;
    for (idx_t i = 0; i < 16; ++i)
        for (idx_t j = 0; j < 16; ++j)
            total += P.m[i][j];

    return total;
}
