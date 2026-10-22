/*
 * A304619  a(n) = 324*n^2 - 804*n + 468 (n>=2).
 * https://oeis.org/A304619
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 324*n^2 - 804*n + 468, with offset 2 (a(2) is the first
 *            term)
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2) {
        return (seq_t)0;          /* values below the offset are irrelevant */
    }
    seq_t nn = (seq_t)n;           /* cast the index to the sequence type */
    seq_t res = (nn * nn * (seq_t)324) - (nn * (seq_t)804) + (seq_t)468;
    return res;
}
