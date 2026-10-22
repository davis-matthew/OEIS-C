/*
 * A364609  a(n) = greatest integer k such that 1/n + 1/(n + 1) + ... + 1/k <
 *          sqrt(2).
 * https://oeis.org/A364609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iteratively find the largest integer k such that
   1/n + 1/(n+1) + ... + 1/k < sqrt(2). */

seq_t a(idx_t n) {
    /* Handle the case below the offset (n < 1). */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Constant sqrt(2) to compare against.  The value
       is accurate enough for the range of n considered. */
    static const double sqrt2 = 1.4142135623730951;

    double sum = 0.0;
    for (uint64_t k = n; ; ++k) {
        sum += 1.0 / (double)k;
        if (sum >= sqrt2) {
            return (seq_t)(k - 1);
        }
    }
}
