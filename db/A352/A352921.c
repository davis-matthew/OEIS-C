/*
 * A352921  Let c(s) denote A109812(s). Suppose c(s) = 2^n - 1, and define
 *          m(n), p(n), r(n) by m(n) = c(s-1)/2^n, p(n) = c(s+1)/2^n, r(n) =
 *          max(m(n), p(n)); sequence gives p(n).
 * https://oeis.org/A352921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n + 1 if n is even and n >= 6, or n = 11; otherwise a(n)
 *            = n
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n >= 6 && (n & 1) == 0) {
        return (seq_t)n + 1;
    }
    if (n == 11) {
        return (seq_t)n + 1;
    }
    return (seq_t)n;
}
