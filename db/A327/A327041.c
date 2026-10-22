/*
 * A327041  a(n) is the number whose binary indices are the union of the
 *          set-system with BII-number n.
 * https://oeis.org/A327041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = The number whose binary indices are the union of the
 *            binary indices of each binary index of n. For n=0, a(0)=0. For
 *            n>0, find positions of 1‑bits in n, for each such position p
 *            find positions of 1‑bits in p, unite all these positions, and
 *            set bits at these positions in the result
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t t = n;
    while (t != 0)
    {
        idx_t lb = t & -t;                               /* lowest set bit of t   */
        idx_t pos = 1 + (idx_t)__builtin_ctzll((unsigned long long)lb);

        /* iterate over set bits of pos */
        idx_t s = pos;
        while (s != 0)
        {
            idx_t s_lb = s & -s;
            idx_t idx = 1 + (idx_t)__builtin_ctzll((unsigned long long)s_lb);
            res |= ((seq_t)1 << (idx - 1));
            s -= s_lb;
        }

        t -= lb;
    }
    return res;
}
