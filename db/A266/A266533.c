/*
 * A266533  First differences of A266532.
 * https://oeis.org/A266533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(1)=1; for n>=2, a(n)=3*(2^{popcount(n-1)}-1)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    idx_t v = n - 1;
    unsigned int pop = 0;
    while (v) {
        pop++;
        v &= v - 1;           /* remove lowest set bit */
    }
    seq_t pow = ((seq_t)1 << pop);   /* 2^pop */
    seq_t val = pow - 1;              /* 2^pop - 1 */
    return (seq_t)3 * val;            /* 3*(2^pop-1) */
}
