/*
 * A368171  a(n) is the smallest divisor d of n such that n/d is a cubefull
 *          exponentially odd number (A335988).
 * https://oeis.org/A368171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(p^e) = p^e if e <= 2, 1 if e > 2 and e odd, p if e > 2 and
 *            even
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Index starts at 1. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Computes the OEIS sequence A368171.
 * a(n) is the smallest divisor d of n such that n/d is a cubefull exponentially odd number.
 * Indexing starts at 1 and all multiplicative rules are respected. */
seq_t a(idx_t n)
{
    seq_t res = 1;
    uint64_t val = (uint64_t)n;
    if (val <= 1)
        return 1;                 /* a(1) = 1 */

    /* Temporary buffers for prime factors and their exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    /* Factor n into prime powers. */
    factor_u64(val, primes, exps, &cnt);

    /* Apply the multiplicative rule. */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];
        if (e <= 2) {
            /* Multiply by p^e. */
            for (uint64_t j = 0; j < e; ++j)
                res *= (seq_t)p;
        } else {
            /* e > 2 */
            if ((e & 1ULL) == 0)   /* e even */
                res *= (seq_t)p;
            /* e odd: contribution is 1, nothing to do */
        }
    }

    return res;
}
