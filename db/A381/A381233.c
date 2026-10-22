/*
 * A381233  Concatenate the sequences S(k) = [0, 1, -1, ..., k, -k] for k = 0,
 *          1, ...
 * https://oeis.org/A381233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 0 if n is a perfect square; * otherwise, let k = ⌊√n⌋,
 *            pos = n - k*k. * Then a(n) = pos == 0 ? 0 : * (pos is odd ?
 *            (pos+1)/2 * : -(pos/2)); * This implements the concatenation of
 *            S(k) = [0,1,-1,…,k,-k] for k=0,1,…
 * Range:     i8 n <= 9157, i16 n <= 9564, i32 n <= 9564, i64 n <= 9564,
 *            i128 n <= 9564, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n) {
    if (n == 0) return 0;
    idx_t low  = 0;
    idx_t high = (n < 0xFFFFFFFFULL) ? n : 0xFFFFFFFFULL;   /* sqrt(n) < 2^32 */
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid == 0) { low = 1; continue; }
        if (mid <= n / mid) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;   /* floor(sqrt(n)) */
}

seq_t a(idx_t n) {
    idx_t k   = isqrt_u64(n);            /* floor(sqrt(n)) */
    idx_t pos = n - k * k;               /* position within S(k) */

    if (pos == 0) return (seq_t)0;
    if (pos & 1U)
        return (seq_t)(pos + 1) / 2;     /* odd position → positive value */
    else
        return -(seq_t)(pos / 2);        /* even position → negative value */
}
