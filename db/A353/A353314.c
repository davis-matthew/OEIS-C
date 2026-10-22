/*
 * A353314  If n is of the form 3k, then a(n) = n, and if n is of the form
 *          3k+r, with r = 1 or 2, then a(n) = 5*k + 3 + r.
 * https://oeis.org/A353314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   if n = 3k then a(n) = n; otherwise a(n) = 5*k + 3 + r , where r
 *            = n mod 3 (r = 1 or 2)
 * Range:     u8 n <= 153, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t k = n / 3;
    idx_t r = n % 3;
    if (r == 0) {
        return (seq_t)n;
    } else {
        return 5 * (seq_t)k + (seq_t)3 + (seq_t)r;
    }
}
