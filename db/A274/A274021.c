/*
 * A274021  Least positive x < n-1 such that x^y == -1 (mod n) for some y > 1,
 *          or 0 if no such x exist.
 * https://oeis.org/A274021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute force search for the least positive x < n‑1 such that
   there exists y > 1 with x^y ≡ -1 (mod n).  If no such x exists, return 0. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n <= 2)
        return 0;

    idx_t mod = n;

    /* Search for x from 1 up to n-2 */
    for (idx_t x = 1; x <= mod - 2; ++x)
    {
        /* Check y = 2 first */
        idx_t val = (x * x) % mod;
        if (val == mod - 1)
            return (seq_t)x;

        /* Check higher exponents */
        for (idx_t y = 3; y <= mod; ++y)
        {
            val = (val * x) % mod;
            if (val == mod - 1)
                return (seq_t)x;
        }
    }

    /* No x found */
    return 0;
}
