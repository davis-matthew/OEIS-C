/*
 * A356389  a(n) = n! * Sum_{k=1..n} ( Sum_{d|k} (-1)^(k/d + 1) ) /k.
 * https://oeis.org/A356389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} (Sum_{d|k} (-1)^(k/d + 1)) / k
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Computes the n-th term of OEIS A356389.
   The function is defined with external linkage and uses only integer
   arithmetic, so it compiles for any seq_t and idx_t defined by the
   build system. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n = 0, return 0 safely */

    /* Allocate an array for the values f(k) = A048272(k). */
    int16_t *f = (int16_t *)calloc((size_t)(n + 1), sizeof(int16_t));
    if (!f) abort();                     /* allocation failure */

    /* Sieve to compute f(k).  For every divisor d of k,
       add +1 if (k/d) is odd, else subtract 1. */
    for (idx_t d = 1; d <= n; ++d) {
        int t = 1;
        for (idx_t m = d; m <= n; m += d, ++t) {
            if (t & 1) ++f[m];           /* odd t → +1 */
            else        --f[m];           /* even t → -1 */
        }
    }

    /* Recurrence:
        a(1) = 1
        a(n) = n * a(n-1) + (n-1)! * f(n)   (for n ≥ 2)          */
    seq_t fact   = (seq_t)1;   /* holds (i-1)! during the loop   */
    seq_t a_prev = (seq_t)1;   /* a(1) = 1                         */
    if (n == 1) {
        free(f);
        return a_prev;
    }

    for (idx_t i = 2; i <= n; ++i) {
        int16_t fn   = f[i];
        seq_t  term  = fact * (seq_t)(fn >= 0 ? fn : -fn); /* (i-1)! * |f(i)| */

        if (fn >= 0)
            a_prev = a_prev * (seq_t)i + term;
        else
            a_prev = a_prev * (seq_t)i - term;

        fact = fact * (seq_t)i;  /* update to i! for next iteration */
    }

    free(f);
    return a_prev;
}
