/*
 * A365684  a(n) is the smallest multiple of n that is an exponentially
 *          squarefree number (A209061).
 * https://oeis.org/A365684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e||n} p^{e + min{k≥0 | e+k is squarefree}}
 * Range:     u8 n <= 143, u16 n <= 9603, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Return true iff m is a squarefree integer. */
static bool is_squarefree_uint64(uint64_t m)
{
    if (m <= 1) return true;
    for (uint64_t i = 2; i * i <= m; ++i) {
        uint64_t sq = i * i;
        if (m % sq == 0) return false;
    }
    return true;
}

/* Smallest integer ≥ e that is squarefree. */
static uint64_t minimal_squarefree_uint64(uint64_t e)
{
    uint64_t x = e;
    for (;;) {
        if (is_squarefree_uint64(x)) return x;
        ++x;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety; n≥1 by problem statement */

    /* Factor n: primes[0..cnt-1], exps[0..cnt-1] */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];
        uint64_t exp = minimal_squarefree_uint64(e);

        seq_t power = (seq_t)1;
        for (uint64_t j = 0; j < exp; ++j)
            power = (seq_t)((unsigned long long)power * p);

        result = (seq_t)((unsigned long long)result * power);
    }
    return result;
}
