/*
 * A253496  Number of (2+1) X (n+1) 0..2 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 400*3^(n-3) + 271*2^(n-1) + 1423 for n > 6 (Hard‑coded
 *            initial values for 1 ≤ n ≤ 6)
 * Range:     u8 none, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Small initial segment (indices 1..6) */
    static const seq_t init[6] = {
        (seq_t)414,   /* a(1) */
        (seq_t)1377,  /* a(2) */
        (seq_t)3090,  /* a(3) */
        (seq_t)5386,  /* a(4) */
        (seq_t)9679,  /* a(5) */
        (seq_t)20975  /* a(6) */
    };

    if (n <= 6)
        return init[n - 1];

    /* Closed‑form formula valid for n > 6 */
    return (seq_t)400 * pow3(n - 3) + (seq_t)271 * pow2(n - 1) + (seq_t)1423;
}
