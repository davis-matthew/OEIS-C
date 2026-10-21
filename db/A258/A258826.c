/*
 * A258826  Numbers m such that k iterations of m under the modified Collatz
 *          function yield k for some k.
 * https://oeis.org/A258826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes by brute‑force search: repeatedly test successive m for the
 * Collatz condition using the modified rule:  f(x)=x/2 if x even,
 * f(x)=(3x+1)/2 if x odd.  Returns the n‑th such m. */
#include <stdbool.h>
#include <stdint.h>

/* test whether m has the required property */
static bool good(seq_t m)
{
    seq_t x = m;
    for (idx_t k = 1;; ++k)
    {
        if ((x & 1) == 0)                /* even */
            x >>= 1;                     /* x = x/2   */
        else
            x = (seq_t)(((__uint128_t)x * 3 + 1) >> 1); /* x = (3x+1)/2 */

        if (x == (seq_t)k)
            return true;

        if (x == 1 && k >= 3)            /* cycle 1↔2 – no further match possible */
            return false;
    }
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t m = 1;                        /* start searching from 2 */
    while (1)
    {
        ++m;
        if (good(m))
        {
            ++count;
            if (count == n)
                return m;
        }
    }
}
