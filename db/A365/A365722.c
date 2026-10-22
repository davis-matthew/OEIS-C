/*
 * A365722  The number of prime factors (with multiplicity) in A356867(1+n),
 *          where A356867 is Sycamore's Doudna variant D(3).
 * https://oeis.org/A365722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of base-3 digits of n minus the number of trailing
 *            2's in its base‑3 representation
 * Range:     u8 n <= 9841, u16 n <= 9841, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t trailing = 0;
    bool counting = true;            /* still looking for trailing 2's */
    idx_t m = n;

    while (m != 0)
    {
        idx_t d = m % 3;              /* least significant base-3 digit */
        sum += (seq_t)d;

        if (counting)                  /* only count 2's at the end */
        {
            if (d == 2)
                trailing++;
            else
                counting = false;       /* first non‑2 digit found */
        }

        m /= 3;
    }

    return sum - trailing;
}
