/*
 * A384558  The sum of the exponential divisors of n that are exponentially
 *          odd numbers (A268335).
 * https://oeis.org/A384558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   multiplicative with a(p^e) = Σ_{d|e , d odd} p^d
 * Range:     u8 n <= 215, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9735 (2026-11-01)
 * Helpers:   factorize, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factorize.h"
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    if (nn == 0) /* offset 1, undefined for 0 – return 0 safely */
        return 0;

    /* Factorize n. For any 64‑bit integer 64 slots are more than enough. */
    uint64_t primes[64];
    int      exps[64];
    size_t   pc = factorize(nn, primes, exps);

    seq_t result = 1;

    for (size_t i = 0; i < pc; ++i)
    {
        seq_t  sum = 0;
        int    e   = exps[i];
        uint64_t p = primes[i];

        /* Sum over odd divisors d of e. */
        for (int d = 1; d <= e; d += 2)
        {
            if (e % d == 0)
                sum += ipow_idx((seq_t)p, (idx_t)d);
        }

        /* Since seq_t may be an integral type or an arbitrary‑precision
           class, the built‑in operators +, * and += work for both. */
        result *= sum;
    }

    return result;
}
