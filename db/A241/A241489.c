/*
 * A241489  Least number k not divisible by 10 such that k^3 contains n zeros.
 * https://oeis.org/A241489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10^(ceil(n/3)+1) + 3 - ((n+2) mod 3) for n > 6. * For n
 *            <= 6 the values are obtained by brute‑force search
 * Range:     u8 none, u16 n <= 1, u32 n <= 24, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   my_pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/my_pow10.h"

/* Count the number of zero digits in v. */
static inline int count_zeros(seq_t v)
{
    int z = 0;
    while (v != 0)
    {
        if ((v % 10) == 0)
            ++z;
        v /= 10;
    }
    return z;
}

/* OEIS A241489 */
seq_t a(idx_t n)
{
    /* Small indices are handled by exhaustive search. */
    if (n <= 6)
    {
        seq_t k = 1;
        while (1)
        {
            if ((k % 10) != 0)
            {
                seq_t sq = k;
                sq *= k;
                sq *= k;
                if (count_zeros(sq) == n)
                    return k;
            }
            ++k;
        }
    }

    /* Large indices use the closed form. */
    idx_t exp = (n + 2) / 3 + 1;          /* ceil(n/3)+1 */
    seq_t val = my_pow10(exp);
    val += (seq_t)3;
    idx_t mod = (n + 2) % 3;
    if (mod)
        val -= (seq_t)mod;
    return val;
}
