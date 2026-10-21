/*
 * A247608  a(n) = Sum_{k=0..3} binomial(6,k)*binomial(n,k).
 * https://oeis.org/A247608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 6*Binomial(n,1) + 15*Binomial(n,2) + 20*Binomial(n,3)
 * Range:     u8 n <= 4, u16 n <= 27, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t res = (seq_t)1;
    seq_t ns = (seq_t)n;
    if (n > 0) {
        res += (seq_t)6 * ns;
    }
    if (n > 1) {
        seq_t t = ns * (ns - (seq_t)1) / (seq_t)2;
        res += (seq_t)15 * t;
    }
    if (n > 2) {
        seq_t t = ns * (ns - (seq_t)1) * (ns - (seq_t)2) / (seq_t)6;
        res += (seq_t)20 * t;
    }
    return res;
}
