/*
 * A379218  Möbius transform of A379108.
 * https://oeis.org/A379218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Möbius transform of A379108. Multiplicative: a(2^e) =
 *            2^(e-1); For odd prime p, a(p^e) = (p^(e+1) + (-1)^e)/(p+1) if p
 *            is a Mersenne prime, a(p^e) = p^e otherwise
 * Range:     u8 n <= 30, u16 n <= 32769, u32 n <= 32769, u64 n <= 32769,
 *            u128 n <= 32769, big any n
 * Validated: standard, pass, 100 terms through n = 9246 (2026-11-01)
 * Helpers:   factor_u64, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"

static bool is_mersenne_prime(uint64_t p)
{
    /* p is odd prime; return true iff p+1 is a power of two */
    uint64_t x = p + 1;
    return (x != 0) && ((x & (x - 1)) == 0);
}

/* Compute a(n) for the OEIS sequence A379218 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* offset is 1; value for n < 1 is undefined */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t part;

        if (p == 2) {
            /* a(2^e) = 2^(e-1) */
            part = ipow_idx((seq_t)2, (idx_t)(e - 1));
        } else {
            if (is_mersenne_prime(p)) {
                /* a(p^e) = (p^(e+1) + (-1)^e)/(p+1) */
                seq_t p_pow = ipow_idx((seq_t)p, (idx_t)(e + 1));
                if ((e & 1) == 0)          /* e even : (+1) */
                    p_pow += (seq_t)1;
                else                       /* e odd  : (-1) */
                    p_pow -= (seq_t)1;
                part = p_pow / ((seq_t)p + 1);
            } else {
                /* a(p^e) = p^e */
                part = ipow_idx((seq_t)p, (idx_t)e);
            }
        }
        result *= part;
    }

    return result;
}
