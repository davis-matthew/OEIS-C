/*
 * A383797  a(n) = 10*binomial(n,5) + 4*binomial(n,3) + n.
 * https://oeis.org/A383797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*binomial(n,5) + 4*binomial(n,3) + n
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 140, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t res = 0;

    /* binomial(n,5) for n >= 5 */
    if (n >= 5) {
        seq_t c5 = 1;
        for (idx_t i = 1; i <= 5; ++i) {
            c5 = c5 * (N - (5 - i));
            c5 = c5 / i;          /* exact division each step */
        }
        res += 10 * c5;
    }

    /* binomial(n,3) for n >= 3 */
    if (n >= 3) {
        seq_t c3 = 1;
        for (idx_t i = 1; i <= 3; ++i) {
            c3 = c3 * (N - (3 - i));
            c3 = c3 / i;
        }
        res += 4 * c3;
    }

    /* add the linear term */
    res += N;
    return res;
}
