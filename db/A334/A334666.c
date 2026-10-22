/*
 * A334666  For any number with binary expansion (b_1, ..., b_w), replace the
 *          i-th "1" by b_i for i = 1..A000120(n) and the j-th "0" by
 *          b_{w+1-j} for j = 1..A023416(n); the resulting binary expansion is
 *          that of a(n).
 * https://oeis.org/A334666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A334666:
   For any number n with binary expansion (b_1,...,b_w),
   a(n) is obtained by replacing the i-th 1 with b_i (i = 1…A000120(n))
   and the j-th 0 with b_{w+1-j} (j = 1…A023416(n)).
   This implementation works for all n ≥ 0. */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* n = 0 is a special case, give 0 immediately */
    if (n == 0)
        return 0;

    /* Determine bit width w of n */
    size_t w = 0;
    idx_t tmp = n;
    while (tmp) {
        w++;
        tmp >>= 1;
    }

    /* Extract original bits b[0..w-1] (MSB first) */
    uint8_t b[64];  /* w ≤ 64 for idx_t = uint64_t */
    for (size_t i = 0; i < w; ++i) {
        b[i] = (n >> (w - 1 - i)) & 1;
    }

    /* Build new bit pattern c[0..w-1] */
    uint8_t c[64];
    size_t ones_seen  = 0; /* i in description */
    size_t zeros_seen = 0; /* j in description */

    for (size_t i = 0; i < w; ++i) {
        if (b[i]) { /* original bit is 1 */
            c[i] = b[ones_seen];
            ones_seen++;
        } else {    /* original bit is 0 */
            c[i] = b[w - 1 - zeros_seen];
            zeros_seen++;
        }
    }

    /* Convert bit array c back to integer value */
    seq_t result = 0;
    for (size_t i = 0; i < w; ++i) {
        result = (result << 1) | (c[i] & 1);
    }

    return result;
}
