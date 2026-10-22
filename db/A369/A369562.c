/*
 * A369562  Smallest positive n-digit number divisible by 7.
 * https://oeis.org/A369562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (floor(10^(n-1)/7)+1)*7, the smallest n‑digit multiple of
 *            7
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"

/* Fast modular exponentiation for small integers. */
static unsigned mod_pow_uint(unsigned base, unsigned exp, unsigned mod)
{
    unsigned result = 1 % mod;
    while (exp)
    {
        if (exp & 1u)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1u;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* offset 1: undefined, safe fallback */

    /* 10^(n-1) as a big or built‑in integer. */
    seq_t p10 = pow10(n - 1);

    /* Compute (10^(n-1) mod 7). Since 10 ≡ 3 (mod 7), this is 3^(n-1) mod 7. */
    unsigned rem = mod_pow_uint(3, (unsigned)(n - 1), 7);

    /* Smallest addition to reach a multiple of 7. */
    unsigned add = (7 - rem) % 7;

    return p10 + (seq_t)add;
}
