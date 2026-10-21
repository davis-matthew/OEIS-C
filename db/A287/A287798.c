/*
 * A287798  Least k such that A006667(k)/A006577(k) = 1/n.
 * https://oeis.org/A287798
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5 * 2^(n-5) for n >= 5; a(3) = 159, a(4) = 6 (offset 3).
 *            This follows the recurrence a(n) = 2*a(n-1) for n > 5
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* handle the special offset terms */
    if (n == 3) return (seq_t)159;
    if (n == 4) return (seq_t)6;
    /* for n >= 5 use the closed‑form value */
    if (n >= 5) {
        idx_t shift = n - 5;          /* exponent of 2 */
        seq_t val = (seq_t)5;
        for (idx_t i = 0; i < shift; ++i) {
            val *= (seq_t)2;          /* safe under unsigned overflow */
        }
        return val;
    }
    /* behavior for n < 3 is undefined; return 0 */
    return (seq_t)0;
}
