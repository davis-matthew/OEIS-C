/*
 * A351416  Number of divisors of n that are either squarefree semiprimes,
 *          numbers of the form p^k (p prime, k>1), or numbers with at least
 *          one square divisor > 1 and 3 or more distinct prime factors.
 * https://oeis.org/A351416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} [[omega(d)=2] = mu(d)^2]
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where [ ] denotes the Iverson bracket.            */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Recursive helper to enumerate all divisors of n */
static void dfs_div(size_t idx, int omega, bool squarefree, size_t prime_cnt,
                    uint64_t exponents[], seq_t *total) {
    if (idx == prime_cnt) {
        /* Evaluate the condition for the current divisor */
        bool cond = squarefree == (omega == 2);
        if (cond) (*total)++;
        return;
    }
    /* For prime idx, choose exponent from 0..exponents[idx] */
    for (int e = 0; e <= (int)exponents[idx]; ++e) {
        bool sf = squarefree;
        int om = omega;
        if (e > 0) {
            om++;
            if (e > 1) sf = false;  /* not squarefree if any exponent > 1 */
        }
        dfs_div(idx + 1, om, sf, prime_cnt, exponents, total);
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;  /* undefined for n=0, but safe */
    /* Factor n.  The arrays are large enough for any 64‑bit integer. */
    size_t prime_cnt = 0;
    uint64_t primes[20];
    uint64_t exponents[20];
    factor_u64(n, primes, exponents, &prime_cnt);

    seq_t total = 0;
    dfs_div(0, 0, true, prime_cnt, exponents, &total);
    return total;
}
