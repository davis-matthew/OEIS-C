/*
 * A363886  Number of divisors of 7*n-3 of form 7*k+5.
 * https://oeis.org/A363886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of (7*n-3) that are ≡ 5 (mod 7)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* compute m = 7*n - 3 using 128‑bit arithmetic to avoid overflow of the
       multiplication before the final cast to uint64_t            */
    unsigned __int128 t = (unsigned __int128)n * 7U - 3U;
    uint64_t m = (uint64_t)t;                     /* m fits in 64 bits for test cases */

    /* factor m (≤ 2^64-1) */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t   pcnt = 0;
    factor_u64(m, primes, exps, &pcnt);

    /* dynamic programming over residue classes modulo 7      */
    seq_t count[7] = {0};
    seq_t newc[7];
    count[1] = 1;                  /* empty product */

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* p ≡ 0 (mod 7) never occurs because m ≡ 4 (mod 7), so this check is
           redundant but harmless                                    */
        if (p % 7U == 0U)
            continue;

        seq_t add[7] = {0};

        /* compute the number of ways to get each residue from this prime */
        int rmod = 1;
        for (uint64_t k = 0; k <= e; ++k) {
            add[rmod] += 1;
            rmod = (int)((rmod * (int)(p % 7U)) % 7);
        }

        /* convolve the current counts with the new contributions   */
        for (int rd = 0; rd < 7; ++rd) newc[rd] = 0;
        for (int r1 = 0; r1 < 7; ++r1) if (count[r1] != 0) {
            for (int r2 = 0; r2 < 7; ++r2) if (add[r2] != 0) {
                int nr = (r1 * r2) % 7;
                newc[nr] += count[r1] * add[r2];
            }
        }
        for (int rd = 0; rd < 7; ++rd) count[rd] = newc[rd];
    }

    return count[5];
}
