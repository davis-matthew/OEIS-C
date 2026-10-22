/*
 * A346975  Expansion of e.g.f. log( 1 + (exp(x) - 1)^3 / 3! ).
 * https://oeis.org/A346975
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = S(n,3) - (1/n) * Σ_{k=1}^{n-1} binomial(n,k) * S(n-k,3) *
 *            k * a(k) (S(n,3) is the Stirling number of the second kind)
 * Range:     i8 n <= 7, i16 n <= 9, i32 n <= 14, i64 n <= 22, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *A = NULL;   /* a(n)   */
static seq_t *B = NULL;   /* n*a(n) */
static seq_t *S2 = NULL;  /* S(n,2) */
static seq_t *S3 = NULL;  /* S(n,3) */
static idx_t cur_max = 0; /* highest n computed */

static inline seq_t
build_S2(idx_t n)
{
    if (n < 2) return (seq_t)0;
    return S2[n];
}

static inline seq_t
build_S3(idx_t n)
{
    if (n < 3) return (seq_t)0;
    return S3[n];
}

seq_t a(idx_t n)
{
    if (n < 3) return 0; /* undefined below the offset */

    /* enlarge storage if necessary */
    if (n > cur_max) {
        idx_t target = n;
        size_t new_cap = (size_t)target + 1;

        A   = (seq_t *)realloc(A,   new_cap * sizeof(seq_t));
        B   = (seq_t *)realloc(B,   new_cap * sizeof(seq_t));
        S2  = (seq_t *)realloc(S2,  new_cap * sizeof(seq_t));
        S3  = (seq_t *)realloc(S3,  new_cap * sizeof(seq_t));

        /* initialise base cases on first call */
        if (cur_max == 0) {
            for (idx_t i = 0; i <= 2; ++i) {
                A[i] = B[i] = S2[i] = S3[i] = (seq_t)0;
            }
        }

        /* compute S(.,2) and S(.,3) up to target */
        for (idx_t i = cur_max + 1; i <= target; ++i) {
            /* S(n,2) = 2*S(n-1,2) + 1 (n>=2) */
            if (i >= 2) {
                S2[i] = S2[i - 1] * (seq_t)2 + (seq_t)1;
            } else {
                S2[i] = (seq_t)0;
            }

            /* S(n,3) = 3*S(n-1,3) + S(n-1,2) (n>=3) */
            if (i >= 3) {
                S3[i] = S3[i - 1] * (seq_t)3 + S2[i - 1];
            } else {
                S3[i] = (seq_t)0;
            }
        }

        /* compute a(k) for k = cur_max+1 .. target */
        for (idx_t k = (cur_max < 3 ? 3 : cur_max + 1); k <= target; ++k) {
            __int128 sum = 0;
            __int128 comb = 1;          /* binomial(k,0) */

            for (idx_t j = 1; j < k; ++j) {
                /* update binomial coefficient: C(k,j) */
                comb = comb * (__int128)(k - j + 1) / (__int128)j;

                /* term = C(k,j) * S(k-j,3) * j*a(j) */
                __int128 term = comb * (__int128)S3[k - j] * (__int128)B[j];
                sum += term;
            }

            seq_t div = (seq_t)(sum / (__int128)k);
            A[k] = S3[k] - div;
            B[k] = (seq_t)((__int128)k * (__int128)A[k]); /* store k*a(k) */
        }

        cur_max = target;
    }

    return A[n];
}
