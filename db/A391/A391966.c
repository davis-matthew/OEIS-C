/*
 * A391966  Smallest integer k greater than 1 such that every solution of y^m
 *          = y (for all integers m > 1) in the commutative ring of n-adic
 *          integers is also a solution of y^k = y.
 * https://oeis.org/A391966
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 if rad(n) = 2; otherwise a(n) = 1 + lcm(p-1 : p divides
 *            rad(n), p <> 2)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"

static seq_t lcm_seq(seq_t a, seq_t b)
{
    seq_t g = gcd(a, b);
    return (a / g) * b;
}

seq_t a(idx_t n)
{
    /* n >= 2 (offset = 2). */

    /* shortcut: power of two → rad(n)=2 */
    if ((n & (n - 1)) == 0)        /* n is a power of two */
        return 3;

    /* remove factor 2 (it is ignored in the lcm computation) */
    while ((n % 2) == 0)
        n /= 2;

    seq_t lcm_val = 1;
    idx_t remaining = n;

    for (idx_t p = 3; p * p <= remaining; p += 2) {
        if (remaining % p == 0) {
            seq_t term = (seq_t)(p - 1);
            lcm_val = lcm_seq(lcm_val, term);
            while (remaining % p == 0)
                remaining /= p;
        }
    }
    if (remaining > 1) {                /* last prime factor > sqrt(original n) */
        seq_t term = (seq_t)(remaining - 1);
        lcm_val = lcm_seq(lcm_val, term);
    }

    return lcm_val + 1;
}
