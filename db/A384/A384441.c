/*
 * A384441  Binary XOR of n and the prime factors of n.
 * https://oeis.org/A384441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = n XOR (XOR of distinct prime factors of n)
 * Range:     u8 n <= 255, u16 n <= 9926, u32 n <= 9926, u64 n <= 9926,
 *            u128 n <= 9926, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Result starts as n itself. */
    seq_t res = (seq_t)n;
    uint64_t tmp = (uint64_t)n;

    /* n = 1 has no prime factors: a(1) = 1 */
    if (tmp <= 1)
        return res;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            /* XOR each distinct prime factor once. */
            res ^= (seq_t)p;
            while (tmp % p == 0)
                tmp /= p;
        }
    }
    /* If n has a remaining prime factor > sqrt(n). */
    if (tmp > 1)
        res ^= (seq_t)tmp;

    return res;
}
