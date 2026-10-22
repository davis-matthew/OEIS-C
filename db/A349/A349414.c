/*
 * A349414  a(n) = A324245(n) - n.
 * https://oeis.org/A349414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n+1)/2 if n odd; -n/4 if n % 4 == 0; ((n-2)/4) - n if n
 *            % 4 == 2
 * Range:     i8 n <= 172, i16 n <= 9676, i32 n <= 9676, i64 n <= 9676,
 *            i128 n <= 9676, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n & 1ULL) {
        seq_t sn = (seq_t)n;
        return (sn + 1) / 2;
    } else {
        seq_t sn = (seq_t)n;
        if ((n & 3ULL) == 0) {
            return -(sn / 4);
        } else {
            return ((sn - 2) / 4) - sn;
        }
    }
}
