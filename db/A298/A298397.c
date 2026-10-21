/*
 * A298397  Pentagonal numbers divisible by 4.
 * https://oeis.org/A298397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*n^2 - 62*n + 40 for n even a(n) = 24*n^2 - 50*n + 26
 *            for n odd (derived from the OEIS definitions)
 * Range:     u8 n <= 4, u16 n <= 53, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert index to seq_t for safe arithmetic */
    seq_t sn = (seq_t)n;
    seq_t temp, result;
    if ((n & 1) == 0) {          /* n is even */
        temp = 24 * sn * sn + 40;
        result = temp - 62 * sn; /* subtraction is safe: temp >= 62*n */
    } else {                     /* n is odd */
        temp = 24 * sn * sn + 26;
        result = temp - 50 * sn; /* subtraction is safe: temp >= 50*n */
    }
    return result;
}
