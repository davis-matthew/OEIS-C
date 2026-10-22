/*
 * A367933  a(n) is the smallest multiple of n that is an exponentially odious
 *          number (A270428).
 * https://oeis.org/A367933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, base
 * Formula:   a(n) = product over prime factors p^e of n of p^s(e), where s(e)
 *            = smallest odd binary‑digit sum exponent >= e
 * Range:     u8 n <= 95, u16 n <= 3124, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static inline int parity_popcount(idx_t u)
{
    int p = 0;
    while (u) {
        p ^= 1;
        u &= u - 1;
    }
    return p;
}

static inline idx_t odious_min(idx_t e)
{
    while (parity_popcount(e) == 0)
        e++;
    return e;
}

static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; i++)
        res *= base;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) /* not defined, but return 0 for completeness */
        return 0;

    /* Factor n; at most 64 distinct primes for any 64‑bit number. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; i++) {
        idx_t e = exps[i];
        idx_t new_e = odious_min(e);
        seq_t pow = pow_seq((seq_t)primes[i], new_e);
        result *= pow;
    }
    return result;
}
