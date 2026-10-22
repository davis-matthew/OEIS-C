/*
 * A360800  Numbers Sum_{i=1..2r+1} 2^k(i) such that k(1) is even and, for r >
 *          0 and i < 2r+1, the difference k(i+1)-k(i) is > 0 and odd.
 * https://oeis.org/A360800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers whose binary representation has an odd number of 1‑bits
 *            and every block of consecutive 0‑bits (including those before
 *            the first 1) has even length. That is equivalent to the OEIS
 *            definition of A360800
 * Range:     u8 n <= 21, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper that tests whether x satisfies the defining properties of A360800
   (can be used for any seq_t type). */
static bool is_valid(seq_t x)
{
    if (x == 0) return false;

    idx_t ones = 0;          /* count of 1‑bits encountered */
    bool seen_one = false;   /* have we seen the first 1 yet? */
    idx_t zero_count = 0;    /* zeros since the last 1 (or from start) */

    /* We process bits from LSB to MSB, keeping the position index i. */
    for (idx_t i = 0; x != 0; ++i)
    {
        bool bit = (x & 1) != 0;   /* current least significant bit */

        if (bit)                    /* we found a 1‑bit */
        {
            if (!seen_one)
            {
                /* The first 1 must appear at an even position. */
                if ((i & 1) != 0) return false;
            }
            else
            {
                /* Between this 1 and the previous one the count of zeros
                   must be even. */
                if ((zero_count & 1) != 0) return false;
            }

            seen_one  = true;
            ones++;
            zero_count = 0;          /* reset zero counter after a 1 */
        }
        else
        {
            if (seen_one) zero_count++;
        }

        x >>= 1;                    /* proceed to next bit */
    }

    /* Number of 1‑bits must be odd. */
    return (ones & 1) != 0;
}

/* Main function that returns the n-th term of A360800 (1‑based index). */
seq_t a(idx_t n)
{
    idx_t  count = 0;
    seq_t  candidate = 1;

    /* Scan successive integers until we have found n valid terms. */
    for (;; ++candidate)
    {
        if (is_valid(candidate))
        {
            ++count;
            if (count == n)
                return candidate;
        }
    }
}
