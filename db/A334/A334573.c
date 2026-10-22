/*
 * A334573  Partial sums of A334572.
 * https://oeis.org/A334573
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=2..n} max(A051903(m), A051903(m-1)) where
 *            A051903(m) is the maximum exponent in the prime factorisation of
 *            m
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* Static state for memoisation */
    static idx_t *spf = NULL;       /* smallest prime factor array */
    static idx_t *pf_exp = NULL;    /* max exponent of prime factors for each i */
    static seq_t *cum  = NULL;      /* cumulative sums a(i) */
    static idx_t current_max = 0;   /* highest n already processed */

    /* Resize / initialise arrays if we need to extend up to n */
    if (spf == NULL || pf_exp == NULL || cum == NULL || current_max < n) {
        idx_t new_size = n + 1;

        spf     = (idx_t *)realloc(spf,     new_size * sizeof(idx_t));
        pf_exp  = (idx_t *)realloc(pf_exp,  new_size * sizeof(idx_t));
        cum     = (seq_t *)realloc(cum,     new_size * sizeof(seq_t));
        if (!spf || !pf_exp || !cum) abort();

        /* Initialise spf values */
        for (idx_t i = 0; i <= n; ++i) spf[i] = i;

        /* Classic SPF sieve */
        for (idx_t i = 2; i * i <= n; ++i) {
            if (spf[i] == i) {
                for (idx_t j = i * i; j <= n; j += i) {
                    if (spf[j] == j) spf[j] = i;
                }
            }
        }

        /* Initialise base cases if this is the first allocation */
        if (current_max == 0) {
            cum[0] = 0;
            cum[1] = 0;
            pf_exp[0] = 0;
            pf_exp[1] = 0;
        }
    }

    /* Compute values from current_max + 1 up to n */
    for (idx_t i = current_max + 1; i <= n; ++i) {
        /* Maximum exponent of a prime divisor of i */
        idx_t tmp = i, local_max = 0;
        while (tmp > 1) {
            idx_t p = spf[tmp];
            idx_t cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            if (cnt > local_max) local_max = cnt;
        }

        seq_t diff;
        if (i == 2) {
            diff = (seq_t)local_max;
        } else {
            diff = (local_max > pf_exp[i - 1]) ? (seq_t)local_max
                                               : (seq_t)pf_exp[i - 1];
        }

        cum[i] = cum[i - 1] + diff;
        pf_exp[i] = local_max;
    }

    current_max = n;
    return cum[n];
}
