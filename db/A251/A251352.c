/*
 * A251352  Number of (n+1)X(1+1) 0..3 arrays with no 2X2 subblock having its
 *          maximum diagonal element less than the absolute difference of its
 *          antidiagonal elements.
 * https://oeis.org/A251352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of (n+1)×(1+1) 0..3 arrays with no 2×2 subblock
 *            having its maximum diagonal element less than the absolute
 *            difference of its antidiagonal elements. The sequence satisfies
 *            the recurrence a(n) = 16*a(n-1) - 40*a(n-2) + 104*a(n-3) -
 *            150*a(n-4) + 118*a(n-5) - 48*a(n-6) + 9*a(n-7)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef struct {
    seq_t m[16][16];
} Matrix;

/* Build the 16×16 adjacency matrix M where M[s][t] = 1 if the transition
   from state s = (a,b) to state t = (c,d) is allowed, otherwise 0.
   States are encoded as s = a*4 + b and t = c*4 + d, with a,b,c,d ∈ {0,1,2,3}. */
static void build_adjacency(Matrix *M)
{
    for (idx_t a = 0; a < 4; ++a)
        for (idx_t b = 0; b < 4; ++b)
            for (idx_t c = 0; c < 4; ++c)
                for (idx_t d = 0; d < 4; ++d)
                {
                    idx_t s = a*4 + b;
                    idx_t t = c*4 + d;
                    seq_t diff = (b > c) ? (b - c) : (c - b);          /* |b-c|          */
                    seq_t maxdiag = (a > d) ? a : d;                   /* max(a,d)        */
                    M->m[s][t] = (maxdiag >= diff) ? 1 : 0;
                }
}

/* Set matrix m to the identity matrix. */
static void identity(Matrix *m)
{
    for (idx_t i = 0; i < 16; ++i)
        for (idx_t j = 0; j < 16; ++j)
            m->m[i][j] = (i == j) ? 1 : 0;
}

/* Matrix multiplication: out = A * B. */
static void mat_mul(const Matrix *A, const Matrix *B, Matrix *out)
{
    for (idx_t i = 0; i < 16; ++i)
        for (idx_t j = 0; j < 16; ++j)
        {
            seq_t sum = 0;
            for (idx_t k = 0; k < 16; ++k)
                sum += A->m[i][k] * B->m[k][j];
            out->m[i][j] = sum;
        }
}

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    static int initialized = 0;
    static Matrix baseM;
    if (!initialized)
    {
        build_adjacency(&baseM);
        initialized = 1;
    }

    Matrix res, expM, temp;
    identity(&res);          /* res = I */
    expM = baseM;            /* expM = M   */

    idx_t exp = n;
    while (exp)
    {
        if (exp & 1ULL)
        {
            mat_mul(&res, &expM, &temp);
            res = temp;
        }
        mat_mul(&expM, &expM, &temp);
        expM = temp;
        exp >>= 1;
    }

    /* Sum of all entries of M^n gives the required count. */
    seq_t total = 0;
    for (idx_t i = 0; i < 16; ++i)
        for (idx_t j = 0; j < 16; ++j)
            total += res.m[i][j];

    return total;
}
