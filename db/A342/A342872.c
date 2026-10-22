/*
 * A342872  Distance to nearest product of 3 consecutive numbers
 *          (three-dimensional promic number, A007531).
 * https://oeis.org/A342872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min_{k ≥ 0} | n - k*(k+1)*(k+2) |
 * Range:     u8 n <= 896, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_cube_root, product_of_three_cube
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses an integer cube‑root to locate the interval containing n. */
#include <stdint.h>

/* Square‑free, but use provided int_cube_root utility. */
#include "../utils/int_cube_root.h"
#include "../utils/product_of_three_cube.h"

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;          /* use seq_t for arithmetic */
    idx_t k   = int_cube_root(n); /* initial naive guess */

    /* Move k down if product exceeds n */
    seq_t tk = product_of_three_cube(k);
    while (tk > ns) {
        k--;
        tk = product_of_three_cube(k);
    }

    /* Now tk <= n. Find next larger product. */
    seq_t tn = product_of_three_cube(k + 1);
    while (tn <= ns) {
        k++;
        tk = tn;
        tn = product_of_three_cube(k + 1);
    }

    /* Compute absolute differences */
    seq_t diff_low, diff_high;
    if (ns >= tk)
        diff_low = ns - tk;
    else
        diff_low = tk - ns;          /* should not happen */

    diff_high = tn - ns; /* tn > ns guaranteed */

    /* Return the smaller difference */
    return diff_low < diff_high ? diff_low : diff_high;
}
