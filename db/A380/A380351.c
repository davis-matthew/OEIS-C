/*
 * A380351  In the ternary expansion of n, from right to left: replace the
 *          first, third, fifth, etc. nonzero digit, says d, by 3-d.
 * https://oeis.org/A380351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = integer whose ternary expansion is obtained from n's
 *            ternary * expansion by replacing the 1st, 3rd, 5th, … nonzero
 *            digits (from the * least‑significant side) d with 3−d
 * Range:     u8 n <= 242, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;      /* copy of n in seq_t type */
    seq_t res   = 0;         /* result accumulator */
    seq_t power = 1;         /* 3^position */
    uint64_t cnt = 0;        /* count of non‑zero digits seen */

    while (x > 0)
    {
        seq_t rem = x % 3;   /* current base‑3 digit */
        x = x / 3;

        seq_t b;
        if (rem == 0)
        {
            b = 0;
        }
        else
        {
            ++cnt;                      /* a non‑zero digit */
            if (cnt & 1)                /* odd occurrence: replace d with 3−d */
                b = 3 - rem;
            else                        /* even occurrence: keep d */
                b = rem;
        }

        res   += power * b;
        power *= 3;
    }

    return res;
}
