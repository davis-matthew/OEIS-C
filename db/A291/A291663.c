/*
 * A291663  Numbers beginning with a vowel in Danish.
 * https://oeis.org/A291663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, word
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 150, u32 n <= 150, u64 n <= 150,
 *            u128 n <= 150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Rule: a(n) is the nth positive integer whose Danish word begins with a vowel.
   In practice this means:
     • 1‑digit numbers: only 1 and 8.
     • 2‑digit numbers: include only those ending in 1 or 8.
     • ≥3‑digit numbers: include all that start with the decimal digit 1 or 8. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;               /* offset starts at 1; safe guard */

    idx_t count = 0u;
    for (idx_t x = 1u; ; ++x)
    {
        bool inc = false;
        if (x < 10u)                          /* single‑digit */
        {
            inc = (x == 1u || x == 8u);
        }
        else if (x < 100u)                    /* two‑digit */
        {
            inc = ((x % 10u) == 1u || (x % 10u) == 8u);
        }
        else                                  /* three or more digits */
        {
            idx_t msd = x;
            while (msd >= 10u)
                msd /= 10u;
            inc = (msd == 1u || msd == 8u);
        }

        if (inc)
        {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
    }
}
