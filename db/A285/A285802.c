/*
 * A285802  Smallest number that when multiplied by n contains the digit 7.
 * https://oeis.org/A285802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = the smallest positive integer m such that the decimal
 *            representation of (n * m) contains the digit 7
 * Range:     u8 n <= 27, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* trivial case: if n is 0, return 0 (undefined in OEIS, but keep it
       safe for our usage).                                           */
    if (n == 0) {
        return (seq_t)0;
    }

    const uint32_t MAX_MULT = 35;          /* conjectured upper bound  */
    seq_t n_seq = (seq_t)n;

    for (uint32_t m = 1; m <= MAX_MULT; ++m) {
        seq_t prod = n_seq * (seq_t)m;
        seq_t tmp  = prod;

        /* scan decimal digits of prod for a 7 */
        while (tmp > 0) {
            if (tmp % 10 == 7) {
                return (seq_t)m;
            }
            tmp /= 10;
        }
    }

    /* According to the conjecture, a solution always exists for 1 <= n.
       This return is only a safety net; it should never be reached.   */
    return (seq_t)MAX_MULT;
}
