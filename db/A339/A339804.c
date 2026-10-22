/*
 * A339804  a(n) = Sum_{k=1..floor(n/2)} k * (n-k) * floor((n-k)/k).
 * https://oeis.org/A339804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..floor(n/2)} k*(n-k)*floor((n-k)/k)
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t lim = n / 2;
    for (idx_t k = 1; k <= lim; ++k)
    {
        idx_t nk = n - k;
        seq_t term = (seq_t)k * (seq_t)nk * (seq_t)(nk / k);
        sum += term;
    }
    return sum;
}
