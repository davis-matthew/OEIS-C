/*
 * A333334  a(n) is the smallest positive number k such that n divides 3^k +
 *          k.
 * https://oeis.org/A333334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest positive k such that n divides 3^k + k (search)
 * Range:     u8 n <= 98, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_mul.h"

seq_t a(idx_t n)
{
    /* The trivial cases. */
    if (n == 1)
        return 1;

    /* If n is a power of 3, the answer is n itself. */
    uint64_t t = n;
    while (t % 3 == 0)
        t /= 3;
    if (t == 1)
        return (seq_t)n;

    /* Iterate over k, keeping 3^k modulo n. */
    idx_t mod_pow = 3 % n;          /* 3^1 mod n */
    idx_t k = 1;

    while (1) {
        /* Check if (3^k + k) mod n == 0. */
        if (((mod_pow + (k % n)) % n) == 0)
            return (seq_t)k;

        /* Prepare next power: 3^(k+1) mod n.  Use mod_mul to avoid overflow. */
        mod_pow = (idx_t)mod_mul((uint64_t)mod_pow, 3, n);
        ++k;
    }
}
