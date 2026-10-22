/*
 * A374989  Square root of largest unitary square divisor of n!.
 * https://oeis.org/A374989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{p ≤ n, v_p(n!) even} p^{v_p(n!)/2}
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 29, u64 n <= 47, u128 n <= 77,
 *            big any n
 * Helpers:   build_primes, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where v_p(n!) = Σ_{k≥1} ⌊n/p^k⌋ */
/* This implements the OEIS sequence A374989. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* Compute the desired a(n). */
seq_t a(idx_t n)
{
    /* static cache of primes up to the greatest n seen */
    static idx_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_cap   = 0;   /* inclusive upper bound used */

    /* enlarge prime table if required */
    if (n > prime_cap) {
        free(primes);                        /* free old table if any */
        build_primes(n, &primes, &prime_cnt); /* allocate new table */
        prime_cap   = n;
    }

    seq_t result = 1;                   /* multiplicative identity */

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        if (p > n) break;                 /* primes array is sorted */

        /* compute v_p(n!) */
        idx_t vp = 0;
        idx_t pw = p;
        while (pw <= n) {
            vp += n / pw;
            if (pw > n / p) break;        /* avoid overflow */
            pw *= p;
        }

        /* include the prime only if the exponent is a positive even number */
        if ((vp & 1U) == 0 && vp > 0) {
            idx_t e = vp / 2;             /* exponent for the square root */
            seq_t mult = seq_pow((seq_t)p, e);
            result = seq_mul(result, mult);
        }
    }

    return result;
}
