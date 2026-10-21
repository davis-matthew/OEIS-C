/*
 * A264852  a(n) = n*(n + 1)*(n + 2)*(9*n - 7)/12.
 * https://oeis.org/A264852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n + 1)*(n + 2)*(9*n - 7)/12
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 151, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t tn  = (seq_t)n;
    seq_t tn1 = tn + 1;
    seq_t tn2 = tn + 2;
    seq_t nine_n       = tn * (seq_t)9;
    seq_t nine_n_minus7 = nine_n - 7;

    seq_t prod = tn * tn1;
    prod = prod * tn2;
    prod = prod * nine_n_minus7;

    return prod / 12;
}
