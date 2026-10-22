/*
 * A338991  a(n) = Sum_{k=1..floor(n/2)} (n-2*k) * floor((n-k)/k).
 * https://oeis.org/A338991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..floor(n/2)} (n-2*k) * floor((n-k)/k)
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = n / 2;
    seq_t N = n;
    for (idx_t k = 1; k <= limit; ++k) {
        seq_t k_seq = k;
        seq_t term = (N - 2 * k_seq) * ((N - k_seq) / k_seq);
        sum += term;
    }
    return sum;
}
