/*
 * A257213  Least d>0 such that floor(n/d) = floor(n/(d+1)).
 * https://oeis.org/A257213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, hear
 * Formula:   a(n) = least d>0 such that floor(n/d) = floor(n/(d+1))
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Special case n == 0: 0/1 == 0/2 */
    if (n == 0)
        return (seq_t)1;

    seq_t sn = (seq_t)n;
    for (idx_t d = 1;; ++d) {
        seq_t q1 = sn / (seq_t)d;
        seq_t q2 = sn / (seq_t)(d + 1);
        if (q1 == q2)
            return (seq_t)d;
    }
}
