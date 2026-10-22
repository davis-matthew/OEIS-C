/*
 * A365837  Largest proper square divisor of n, for n >= 2; a(1) = 1.
 * https://oeis.org/A365837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest proper square divisor of n, with a(1)=1
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;

    uint64_t temp = (uint64_t)n;
    seq_t largest_sq = 1;
    bool is_square = true;
    uint64_t smallest_pf = 0;

    /* prime factorisation */
    for (uint64_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p != 0)
            continue;

        uint64_t exp = 0;
        while (temp % p == 0)
        {
            temp /= p;
            ++exp;
        }

        /* multiply by p^(2*floor(exp/2)) */
        uint64_t sq_exp = (exp / 2) * 2;
        for (uint64_t i = 0; i < sq_exp; ++i)
            largest_sq *= (seq_t)p;

        if (smallest_pf == 0 || p < smallest_pf)
            smallest_pf = p;

        if (exp & 1)
            is_square = false;
    }

    /* leftover prime factor > sqrt(original n) */
    if (temp > 1)
    {
        /* exponent = 1, so it destroys squareness */
        if (smallest_pf == 0 || temp < smallest_pf)
            smallest_pf = temp;
        is_square = false;
    }

    /* if n was a perfect square, remove the factor p^2 of its
       smallest prime factor to obtain the largest proper square divisor */
    if (is_square)
    {
        seq_t remove = (seq_t)smallest_pf * (seq_t)smallest_pf;
        largest_sq /= remove;
    }

    return largest_sq;
}
