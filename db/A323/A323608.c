/*
 * A323608  The position function the fractalization of which yields A323607.
 * https://oeis.org/A323608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   * Let n = 2^c * x with x odd. * If x = 1 (n is a power of two)
 *            -> a(n) = 2^c - c * If x ≥ 3 (odd part) -> a(n) = ((2^{c+1}-1) *
 *            x - (2c+1)) / 2 * * The function implements the above piecewise
 *            formula in O(1) time
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, if needed in the build */

seq_t a(idx_t n) {
    /* n ≥ 1, so __builtin_ctzll is safe */
    idx_t c = __builtin_ctzll((unsigned long long)n);  /* exponent of 2 */
    idx_t odd = n >> c;                                /* odd part */

    if (odd == 1) {
        /* n = 2^c */
        // use idx_t for the shift, cast to seq_t for the final result
        seq_t res = (seq_t)(((idx_t)1 << c) - c);
        return res;
    } else {
        /* odd part ≥ 3 */
        idx_t mul = (((idx_t)1 << (c + 1)) - 1);               // 2^{c+1} - 1
        idx_t numer = mul * odd - (2 * c + 1);                // numerator
        seq_t res = (seq_t)(numer / 2);                      // division by 2
        return res;
    }
}
