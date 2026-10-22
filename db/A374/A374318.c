/*
 * A374318  For any n > 0, let b_n(n+1) = 0, and for k = 1..n, if b_n(k+1) >=
 *          k then b_n(k) = b_n(k+1) - k otherwise b_n(k) = b_n(k+1) + k; a(n)
 *          = b_n(1).
 * https://oeis.org/A374318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; * For n>0, let b_n(n+1)=0 and for k=n down to 1 * if
 *            b_n(k+1) >= k then b_n(k) = b_n(k+1) - k * else b_n(k) =
 *            b_n(k+1) + k * Then a(n)=b_n(1)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t prev = 0;
    for (idx_t k = n; k > 0; --k) {
        if (prev >= (seq_t)k)
            prev -= (seq_t)k;
        else
            prev += (seq_t)k;
    }
    return prev;
}
