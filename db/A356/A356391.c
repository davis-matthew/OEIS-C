/*
 * A356391  a(n) = n! * Sum_{k=1..n} ( Sum_{d|k} (-1)^(k/d + 1) * d^2 ) /k.
 * https://oeis.org/A356391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Implementation of OEIS A356391:
 * a(n) = n! * Sum_{k=1..n} ( Sum_{d|k} (-1)^(k/d + 1) * d^2 ) / k.
 * Equivalent recurrence:
 *   a(1) = 1
 *   a(n) = n * a(n-1) + (n-1)! * A(n)   for n >= 2
 * where A(n) = Sum_{d|n} (-1)^(n/d + 1) * d^2.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for offset 1, but harmless */
    /* Allocate array for A(n) values as int64_t to avoid unsigned subtraction */
    size_t sz = (size_t)(n + 1);
    int64_t *A = (int64_t *)calloc(sz, sizeof(int64_t));
    if (!A) return (seq_t)0;   /* allocation failure handling */

    /* Sieve to compute A(k) for all k <= n */
    for (idx_t d = 1; d <= n; ++d) {
        int64_t d2 = (int64_t)d * (int64_t)d;
        for (idx_t m = d; m <= n; m += d) {
            idx_t q = m / d;                         /* k/d */
            /* (-1)^(k/d + 1) : even exponent -> +1, odd -> -1 */
            if ( ((q + 1) & 1ULL) == 0 ) { /* even exponent -> +1 */
                A[m] += d2;
            } else {                      /* odd exponent -> -1 */
                A[m] -= d2;
            }
        }
    }

    /* Compute a(n) iteratively using the recurrence */
    seq_t factorial = (seq_t)1;   /* (n-1)! during i-th iteration */
    seq_t a_prev     = (seq_t)1;   /* a(1) */

    if (n == 1) {
        free(A);
        return a_prev;
    }

    for (idx_t i = 2; i <= n; ++i) {
        seq_t An = (seq_t)A[(size_t)i];          /* A(i) as seq_t */
        a_prev = (seq_t)i * a_prev + factorial * An;
        factorial *= (seq_t)i;
    }

    free(A);
    return a_prev;
}
