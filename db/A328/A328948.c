/*
 * A328948  Number of primes that are a concatenation of two positive integers
 *          whose product is n.
 * https://oeis.org/A328948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of distinct primes that can be written as the
 *            concatenation * of two positive integers d and n/d, where d
 *            divides n
 * Range:     u8 n <= 26, u16 n <= 4757, u32 n <= 6981, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   concat_parts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1 and a(3n + 2) = 0 by definition. */
    if (n < 1u)
        return 0;
    if (n % 3u == 2u)
        return 0;

    /* The number of distinct primes that can appear for n up to about 10 000 is
     * very small; 128 slots are more than enough for any n that fits in uint64_t. */
    seq_t primes[128];
    size_t count = 0;

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0)
            continue;
        idx_t e = n / d;

        /* Check the pair (d , e). */
        seq_t p = concat_parts(d, e);
        if (is_prime_big(p))
        {
            bool dup = false;
            for (size_t i = 0; i < count; ++i)
                if (primes[i] == p) { dup = true; break; }
            if (!dup)
                primes[count++] = p;
        }

        /* For d ≠ e also check the reversed pair (e , d). */
        if (d != e)
        {
            seq_t p2 = concat_parts(e, d);
            if (is_prime_big(p2))
            {
                bool dup = false;
                for (size_t i = 0; i < count; ++i)
                    if (primes[i] == p2) { dup = true; break; }
                if (!dup)
                    primes[count++] = p2;
            }
        }
    }

    return (seq_t)count;
}
