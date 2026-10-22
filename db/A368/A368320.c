/*
 * A368320  Expansion of e.g.f. exp(3*x) / (3 - 2*exp(x)).
 * https://oeis.org/A368320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 3^n + 2 * Σ_{k=1..n} C(n,k) * a(n-k) (n ≥ 1)
 *            (Here C(n,k) denotes the binomial coefficient.)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>   /* size_t */

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static idx_t  size       = 0;    /* number of computed values */
    static idx_t  cap        = 0;    /* allocated capacity */
    static seq_t  pow3       = 1;    /* 3^k for k = size-1 (initially 3^0) */
    static bool   initialized = false;

    /* one‑time initialization */
    if (!initialized) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);           /* allocation failure */
        cache[0] = (seq_t)1;           /* a(0) */
        size = 1;
        initialized = true;
    }

    /* compute all needed values up to n */
    if (n >= size) {
        for (idx_t m = size; m <= n; ++m) {
            /* update current power of 3: pow3 = 3^m */
            pow3 = pow3 * (seq_t)3;

            /* compute the sum Σ_{j=0..m-1} C(m,j) * a(j) */
            seq_t sum  = (seq_t)0;
            seq_t binom = (seq_t)1;          /* C(m,0) */

            for (idx_t j = 0; j < m; ++j) {
                sum += binom * cache[j];
                /* update binom to C(m,j+1) */
                binom = (binom * (seq_t)(m - j)) / (seq_t)(j + 1);
            }
            sum = sum * (seq_t)2;         /* multiply by 2 */

            /* a(m) = 3^m + 2 * Σ C(m,j)*a(j) */
            seq_t am = pow3 + sum;

            /* enlarge cache if necessary */
            if (m >= cap) {
                size_t new_cap = cap ? cap : 4;
                while (new_cap <= (size_t)m) new_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                cache = tmp;
                cap = (idx_t)new_cap;
            }
            cache[m] = am;
        }
        size = n + 1;
    }

    return cache[n];
}
