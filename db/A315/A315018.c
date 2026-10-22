/*
 * A315018  Coordination sequence Gal.6.193.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=a(n-1)+d(n) for n>0
 *            d(1..12)=[4,4,6,4,4,5,5,4,4,6,4,5]; d(n>=13) repeats with period
 *            12: [5,4,6,4,4,5,5,4,4,6,4,5]
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init_prefix[13] = {
    0, 4, 8, 14, 18, 22, 27, 32, 36, 40, 46, 50, 55
};

static const seq_t period[12] = {
    5, 4, 6, 4, 4, 5, 5, 4, 4, 6, 4, 5
};

static const seq_t period_prefix[13] = {
    0, 5, 9, 15, 19, 23, 28, 33, 37, 41, 47, 51, 56
};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if (n <= 12)
        return (seq_t)1 + init_prefix[(size_t)n];

    /* a(12) = 56 */
    seq_t result = (seq_t)56;

    idx_t n_left = n - 12;
    idx_t cycles = n_left / 12;
    idx_t rem    = n_left % 12;

    result += (seq_t)cycles * (seq_t)56;      /* each full cycle adds 56 */
    result += period_prefix[(size_t)rem];     /* partial cycle */

    return result;
}
