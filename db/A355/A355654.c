/*
 * A355654  For any number n with runs in binary expansion (r_w, ..., r_0),
 *          let p(n) be the polynomial of a single indeterminate x where the
 *          coefficient of x^e is r_e for e = 0..w and otherwise 0, and let q
 *          be the inverse of p; a(n) = q(p(n)^2).
 * https://oeis.org/A355654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) equals the integer whose binary run–lengths are * the
 *            coefficients of the square of the polynomial formed by the *
 *            binary run–lengths of n. (See OEIS A355654.)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 31, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   encode_from_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/encode_from_runs.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* base case */
        return (seq_t)0;

    /* ---------- 1. Extract runs of n (LSB first) --------------------- */
    idx_t runs[65];          /* enough for 64‑bit numbers */
    size_t r_len = 0;

    idx_t tmp      = n;
    int    cur_bit = tmp & 1;          /* LSB bit */
    idx_t  run_len = 0;

    while (tmp != 0) {
        int bit = tmp & 1;
        if (bit == cur_bit) {
            ++run_len;
        } else {
            runs[r_len++] = run_len;
            run_len       = 1;
            cur_bit       = bit;
        }
        tmp >>= 1;
    }
    runs[r_len++] = run_len;            /* final run */

    /* ---------- 2. Convolve to square the polynomial ---------------- */
    size_t c_len = 2 * r_len - 1;
    idx_t  coeffs[129] = {0};          /* 2*64 - 1 fits in 129 */

    for (size_t i = 0; i < r_len; ++i) {
        for (size_t j = 0; j < r_len; ++j) {
            coeffs[i + j] += runs[i] * runs[j];
        }
    }

    /* ---------- 3. Encode the coefficient array into an integer ------ */
    return encode_from_runs(coeffs, c_len);
}
