/*
 * A261418  The UUDDUUD sequence, or the 3/7 sequence: start with 4, then
 *          successively add or subtract the integers 1,2,3,..., using the
 *          repeating sign pattern + + - - + + -.
 * https://oeis.org/A261418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: UUDDUUD (3/7) – start at 4 and add/subtract 1,2,3,... 
 * with repeating sign pattern + + - - + + -.  We compute 
 * a(n) efficiently using the observation that the sum over each 
 * full 7‑term block is exactly 7m, where m = floor(n/7).
 * a(7m) = 4 + 7 * m * (m-1)/2.
 * The remaining r = n % 7 terms are processed one by one.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert n to a signed-like intermediate for m calculation */
    idx_t m = n / 7;
    idx_t r = n % 7;

    /* base = 4 + 7 * m * (m-1) / 2; use seq_t for all arithmetic */
    seq_t base = (seq_t)4;
    seq_t temp = (seq_t)m * (seq_t)(m - 1) / 2;   /* m*(m-1)/2 */
    base += (temp * (seq_t)7);

    seq_t result = base;

    /* Process the remaining r terms */
    for (idx_t i = 1; i <= r; ++i) {
        idx_t k = 7 * m + i;
        idx_t rem = i;                     /* sign depends on i (pattern repeats every 7) */
        if (rem == 1 || rem == 2 || rem == 5 || rem == 6) {
            result += (seq_t)k;          /* plus */
        } else {
            result -= (seq_t)k;          /* minus – safe because result >= k */
        }
    }

    return result;
}
