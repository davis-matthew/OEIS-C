/*
 * A356004  a(n) = n! * Sum_{k=1..n} Sum_{d|k} 1/(d! * (k/d)!).
 * https://oeis.org/A356004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} Sum_{d|k} 1/(d! * (k/d)!)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Return a(n) for n >= 1.
   Uses memoisation and the recurrence
      a(1) = 1,
      a(n) = n * a(n-1) + n! * Sum_{d|n} 1/(d! * (n/d)!)          */
seq_t a(idx_t n) {
    static seq_t *fact = NULL;       /* factorials 0..max_n      */
    static seq_t *A    = NULL;       /* sequence values a(1)..a(max_n) */
    static idx_t max_n_cached = 0;   /* highest n for which A and fact are valid */

    if (n == 0) return (seq_t)0; /* undefined for 0, return 0 */

    /* initialise for n==1 first time */
    if (max_n_cached < 1) {
        fact = (seq_t*)malloc((1 + 1) * sizeof(seq_t));
        A    = (seq_t*)malloc((1 + 1) * sizeof(seq_t));
        fact[0] = (seq_t)1;
        fact[1] = (seq_t)1;
        A[0]    = (seq_t)0;
        A[1]    = (seq_t)1;
        max_n_cached = 1;
    }

    /* extend cache up to requested n */
    if (max_n_cached < n) {
        idx_t old = max_n_cached;
        idx_t new_n = n;

        /* resize arrays */
        fact = (seq_t*)realloc(fact, (new_n + 1) * sizeof(seq_t));
        A    = (seq_t*)realloc(A,    (new_n + 1) * sizeof(seq_t));

        /* compute new factorials */
        for (idx_t i = old + 1; i <= new_n; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }

        /* compute new sequence values */
        for (idx_t k = (old < 1 ? 1 : old + 1); k <= new_n; ++k) {
            /* compute R(k) = k! * sum_{d|k} 1/(d! * (k/d)!) */
            seq_t R = (seq_t)0;
            for (idx_t d = 1; d * d <= k; ++d) {
                if (k % d == 0) {
                    idx_t m = k / d;
                    seq_t term = fact[k];
                    term /= fact[d];
                    term /= fact[m];

                    R += term;
                    if (d != m) {
                        /* term is symmetric for divisor pair (d,m) */
                        R += term;
                    }
                }
            }
            A[k] = (seq_t)k * A[k - 1] + R;
        }
        max_n_cached = new_n;
    }

    return A[n];
}
