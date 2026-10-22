/*
 * A365478  In the Collatz problem, largest value in the trajectory of n in
 *          the 3x+1 function (denoted by T(x) in the literature, and defined
 *          as T(x) = (3x+1)/2 if x is odd, T(x) = x/2 if x is even), or -1 if
 *          the trajectory is divergent.
 * https://oeis.org/A365478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 702, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of the maximum excursion value in the Collatz trajectory
   using the function T(x) = (3x+1)/2 if x is odd, T(x) = x/2 if x is even. */
seq_t a(idx_t n) {
    seq_t cur = (seq_t)n;
    seq_t max = cur;
    while (cur > (seq_t)1) {
        if (cur & (seq_t)1) {
            cur = ((cur * (seq_t)3 + (seq_t)1) >> 1);
        } else {
            cur >>= 1;
        }
        if (cur > max) {
            max = cur;
        }
    }
    return max;
}
