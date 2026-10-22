/*
 * A322674  Square array read by antidiagonals: T(n, k) = 1 if the digits of p
 *          = n*k in base 2 are exactly the same as the digits of p when
 *          considering the base-2 representations of n, k and p as base-10
 *          numbers, otherwise T(n, k) = 0.
 * https://oeis.org/A322674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base, tabl
 * Formula:   a(n) = 1 when the decimal value of the binary digits of i*j *
 *            equals the product of the decimal values of the binary digits of
 *            i and j, * where (i,j) are the antidiagonal coordinates of index
 *            n; otherwise 0
 * Range:     u8 n <= 22, u16 n <= 3585, u32 n <= 7178, u64 n <= 7178,
 *            u128 n <= 7178, big any n
 * Helpers:   antidiagonal_coords, floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/antidiagonal_coords.h"
#include "../utils/floor_log2_uint64.h"

static seq_t dec_from_bin_idx(idx_t val) {
    if (val == 0) return (seq_t)0;
    seq_t dec = (seq_t)0;
    int pos = floor_log2_uint64(val);
    for (; pos >= 0; --pos) {
        seq_t bit = (seq_t)((val >> pos) & 1u);
        dec = dec * (seq_t)10 + bit;
    }
    return dec;
}

seq_t a(idx_t n) {
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    seq_t p = (seq_t)i * (seq_t)j;
    seq_t dn = dec_from_bin_idx(i);
    seq_t dk = dec_from_bin_idx(j);
    seq_t dp = dec_from_bin_idx((idx_t)p);
    return (dn * dk == dp) ? (seq_t)1 : (seq_t)0;
}
