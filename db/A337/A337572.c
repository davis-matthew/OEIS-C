/*
 * A337572  Numbers having at least one 4 in their representation in base 5.
 * https://oeis.org/A337572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = numbers containing at least one digit 4 in base‑5
 *            representation
 * Range:     u8 n <= 123, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9885 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* helper that returns true if a base‑5 representation of x contains a 4 */
static inline bool hasFourBase5(seq_t x)
{
    while (x > 0)
    {
        if ((x % 5) == 4)
            return true;
        x /= 5;
    }
    return false;
}

/* returns the n-th positive integer whose base‑5 representation contains a digit 4 */
seq_t a(idx_t n)
{
    seq_t  x = 1;          /* current integer being tested */
    idx_t  count = 0;      /* how many qualifying numbers found so far */

    while (1)
    {
        if (hasFourBase5(x))
        {
            ++count;
            if (count == n)
                return x;
        }
        ++x;
    }
}
