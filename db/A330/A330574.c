/*
 * A330574  a(n) = r_2(n)^2*d(n+1), where r_2 = A004018, d = A000005.
 * https://oeis.org/A330574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = r_2(n)^2 * d(n+1) * where r_2(n) is the number of
 *            representations of n as a sum * of two squares (including order
 *            and sign) and * d(m) is the number of divisors of m
 * Range:     u8 n <= 4, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache of precomputed values */
    static idx_t cache_lim = 0;          /* largest n for which arrays are filled   */
    static seq_t *r2  = NULL;            /* r_2(n) values                         */
    static seq_t *div = NULL;            /* divisor counts d(m)                   */
    static idx_t *d1  = NULL;            /* count of divisors ≡ 1 (mod 4)          */
    static idx_t *d3  = NULL;            /* count of divisors ≡ 3 (mod 4)          */

    /* The special case n = 0: r_2(0) = 1, d(1) = 1. */
    if (n == 0) return (seq_t)1;

    idx_t needed = n + 1;                /* indices required up to n+1              */
    if (needed > cache_lim) {
        /* Allocate space for indices 0 … needed */
        size_t sz = (size_t)needed + 1U;
        free(r2);   free(div);  free(d1);  free(d3);
        r2  = (seq_t*)calloc(sz, sizeof(seq_t));
        div = (seq_t*)calloc(sz, sizeof(seq_t));
        d1  = (idx_t*)calloc(sz, sizeof(idx_t));
        d3  = (idx_t*)calloc(sz, sizeof(idx_t));
        if (!r2 || !div || !d1 || !d3) return (seq_t)0;   /* allocation failure */

        /* Sieve to fill divisor counts and d1/d3 counters */
        for (idx_t i = 1; i <= needed; ++i) {
            for (idx_t j = i; j <= needed; j += i) {
                div[j] += 1U;
            }
            if ((i & 3) == 1) {                       /* i ≡ 1 (mod 4) */
                for (idx_t j = i; j <= needed; j += i)
                    d1[j] += 1U;
            } else if ((i & 3) == 3) {               /* i ≡ 3 (mod 4) */
                for (idx_t j = i; j <= needed; j += i)
                    d3[j] += 1U;
            }
        }

        /* Compute r_2(n) from d1 and d3 counts */
        r2[0] = (seq_t)1;                 /* special value for n = 0 */
        for (idx_t i = 1; i <= needed; ++i) {
            seq_t val = (seq_t)4 *
                        ((seq_t)d1[i] - (seq_t)d3[i]);
            r2[i] = val;
        }
        cache_lim = needed;
    }

    /* a(n) = r_2(n)^2 * d(n+1) */
    seq_t res = r2[n];
    res = res * res;           /* square r_2(n) */
    res = res * div[n + 1];    /* multiply by d(n+1) */
    return res;
}
