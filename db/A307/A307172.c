/*
 * A307172  Second class of all proper positive solutions x2(n) of the Pell
 *          equation x^2 - 7*y^2 = 9.
 * https://oeis.org/A307172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=4, a(2)=53; a(n)=16*a(n-1)-a(n-2) for n>=3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        /* undefined for the offset, but return 0 as a safe placeholder */
        return 0;
    }
    if (n == 1) {
        return (seq_t)4;
    }
    if (n == 2) {
        return (seq_t)53;
    }
    seq_t a_prev2 = (seq_t)4;
    seq_t a_prev1 = (seq_t)53;
    seq_t a_curr;
    for (idx_t i = 3; i <= n; ++i) {
        a_curr = (seq_t)16 * a_prev1 - a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
