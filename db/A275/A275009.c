/*
 * A275009  Table of F[k]_n(2) with rows k >= 0 and columns 0 <= n <= 2.
 * https://oeis.org/A275009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   let k = n / 3. Then a(n) = k + 2 if n % 3 == 0 2 * (k + 1) if n
 *            % 3 == 1 2 * (k + 1)^2 if n % 3 == 2 (row‑major order of the
 *            table F[k]_n(2) with columns n = 0, 1, 2)
 * Range:     u8 n <= 34, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t k   = n / 3;
    idx_t rem = n % 3;
    seq_t result;

    if (rem == 0) {
        result = (seq_t)(k + 2);
    } else if (rem == 1) {
        result = (seq_t)2 * (seq_t)(k + 1);
    } else { /* rem == 2 */
        seq_t kp1 = (seq_t)(k + 1);
        result = (seq_t)2 * kp1 * kp1;
    }
    return result;
}
