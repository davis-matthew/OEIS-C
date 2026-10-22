/*
 * A353529  a(n) = 1 if A053669(n) [the smallest prime not dividing n] is of
 *          the form 4m+3, otherwise a(n) = 0.
 * https://oeis.org/A353529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if smallest_prime_not_dividing(n) ≡ 3 (mod 4), else 0
 * Range:     u8 n <= 9887, u16 n <= 9887, u32 n <= 9887, u64 n <= 9887,
 *            u128 n <= 9887, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* helper: check primality of a small integer */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2)          return false;
    if (x == 2 || x == 3) return true;
    if (x % 2 == 0)     return false;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    uint64_t val = (uint64_t)n;   /* n is the index, no special conversion needed */
    uint64_t p = 2;               /* smallest prime candidate */

    for (;; ++p) {
        if (!is_prime_uint64(p))
            continue;            /* skip composite numbers */

        if (val % p != 0) {      /* p is the smallest prime that does NOT divide n */
            return (seq_t)((p % 4) == 3);
        }
    }
}
