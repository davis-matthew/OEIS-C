/*
 * A385731  Number of divisors d of n such that (-d) == (-d)^d == d^d (mod n).
 * https://oeis.org/A385731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that (-d) ≡ (-d)^d ≡ d^d
 *            (mod n)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation enumerates all divisors and checks the condition using the provided mod_pow. */
#include <stdbool.h>
#include "../utils/mod_pow.h"

static bool good_divisor(idx_t n, idx_t d)
{
    seq_t mod = (seq_t)n;
    seq_t base_d = (seq_t)d;
    seq_t A = mod_pow(base_d, (uint64_t)d, mod);
    seq_t md = mod - base_d;
    seq_t B = mod_pow(md, (uint64_t)d, mod);
    return (A == B) && (A == md);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t count = 0;
    /* compute floor(sqrt(n)) safely for any 0 < n <= UINT64_MAX */
    idx_t r = 0;
    while ((r + 1) <= n / (r + 1)) ++r;
    for (idx_t d = 1; d <= r; ++d) {
        if (n % d) continue;
        if (good_divisor(n, d)) ++count;
        idx_t d2 = n / d;
        if (d2 != d && good_divisor(n, d2)) ++count;
    }
    return count;
}
