/*
 * A323136  a(n) is the least power of a prime that n reaches under the map x
 *          -> A070229(x), or -1 if we never reach a power of a prime.
 * https://oeis.org/A323136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least power of a prime reached by repeatedly applying
 *            the map x → x + A070229_largest_prime_factor(x), where
 *            A070229_largest_prime_factor(x) is the largest prime dividing x.
 *            a(n) is computed for all n ≥ 1
 * Range:     u8 n <= 33, u16 n <= 513, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"

static bool is_power_of_prime_uint64(uint64_t x)
{
    if (x <= 1)  /* 1 is treated as a prime power */
        return true;

    size_t cnt = 0;
    uint64_t p[20], e[20];
    factor_u64(x, p, e, &cnt);
    return cnt == 1;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n = 0 but returned for safety */

    seq_t cur = (seq_t)n;

    while (true)
    {
        uint64_t cur64 = (uint64_t)cur;

        if (is_power_of_prime_uint64(cur64))
            return cur;

        size_t cnt = 0;
        uint64_t p[20], e[20];
        factor_u64(cur64, p, e, &cnt);

        /* largest prime factor is the last in the array (sorted ascending) */
        uint64_t largest = p[cnt - 1];

        cur64 += largest;
        cur = (seq_t)cur64;
    }
}
