/*
 * A314501  Coordination sequence Gal.6.478.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; If n is even > 0: let k = n/2, q = ⌊(k-1)/3⌋, r =
 *            (k-1) mod 3; a(n) = 8 + 26*q + 10*(r>0) + 8*(r>1). If n is odd:
 *            let k = (n-1)/2, q = ⌊k/6⌋, r = k mod 6; a(n) = 5 + 59*q +
 *            {0,9,21,28,40,49}[r]
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    if ((n & 1ULL) == 0) /* even n > 0 */
    {
        idx_t k = n >> 1;            /* k = n/2 */
        idx_t k_minus1 = k - 1;      /* k >= 1 */
        idx_t q = k_minus1 / 3;
        idx_t r = k_minus1 % 3;

        seq_t val = 8;
        val += (seq_t)26 * (seq_t)q;
        if (r >= 1) val += 10;
        if (r >= 2) val += 8;
        return val;
    }
    else /* odd n */
    {
        idx_t k = n >> 1;           /* k = (n-1)/2 */
        idx_t q = k / 6;
        idx_t r = k % 6;

        /* partial sums for r = 0..5 : 0,9,21,28,40,49 */
        static const seq_t partial[6] = {0, 9, 21, 28, 40, 49};

        seq_t val = 5 + (seq_t)59 * (seq_t)q + partial[r];
        return val;
    }
}
