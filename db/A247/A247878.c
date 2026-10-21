/*
 * A247878  For bases b = 2, 3, ..., n, let the base-b expansion of n be
 *          [c_{1,b} c_{2,b} .. c_{r_b,b}], with the most significant "digit"
 *          on the left, 0 <= c_{i,b} < b, and c_{1,b} != 0; then a(n) is the
 *          number whose base-n expansion is c_{1,2} c_{2,2} ... c_{r_2,2}
 *          c_{1,3} ... c_{1,n} c_{2,n} ... c_{r_n,n}.
 * https://oeis.org/A247878
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A247878
 *  For n ≥ 2, a(n) is obtained by concatenating the base‑b
 *  representations of n for all b = 2, 3, … , n and interpreting
 *  the resulting string as digits in base n.
 */
#include <stdint.h>  /* for uint8_t */

seq_t a(idx_t n) {
    /* a(2) is the first term in the OEIS list; for n < 2
     * the value is not defined, but we return 0 safely. */
    if (n < 2) return 0;

    seq_t result = 0;
    const seq_t n_seq = (seq_t)n;          /* base for final representation */

    /* Temporary storage for digits; maximum digits needed are
     * bounded by ceil(log2(n)) + 1, which is < 64 for all n
     * represented in the supported builds. */
    uint8_t digits[64];

    for (idx_t base = 2; base <= n; ++base) {
        /* get digits of n in base 'base' in reverse order */
        idx_t temp = n;
        idx_t cnt = 0;
        while (temp > 0 && cnt < 64) {
            digits[cnt++] = (uint8_t)(temp % base);
            temp /= base;
        }
        /* process digits from most significant to least major,
         * accumulating the result as a number in base n */
        for (idx_t i = cnt; i > 0; --i) {
            result = result * n_seq + digits[i - 1];
        }
    }
    return result;
}
