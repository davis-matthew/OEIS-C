/*
 * A350162  a(n) = Sum_{k=1..n} (-1)^(k+1) * floor(n/(2*k-1))^2.
 * https://oeis.org/A350162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k+1) * floor(n/(2*k-1))^2
 * Range:     u8 n <= 16, u16 n <= 267, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t v = (seq_t)n / (2*k - 1);
        seq_t v2 = v * v;
        if (k & 1)        /* k is odd -> add */
            result += v2;
        else              /* k is even -> subtract */
            result -= v2;
    }
    return result;
}
