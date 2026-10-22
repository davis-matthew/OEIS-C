/*
 * A350163  a(n) = Sum_{k=1..n} (-1)^(k+1) * floor(n/(2*k-1))^3.
 * https://oeis.org/A350163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k+1) * floor(n/(2*k-1))^3
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    long long sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t denom = (k << 1) - 1;          /* 2*k - 1 */
        idx_t q = n / denom;                 /* floor division */
        long long val = (long long)q * q * q; /* q^3 */
        if (k & 1)                            /* k odd -> positive term */
            sum += val;
        else                                  /* k even -> negative term */
            sum -= val;
    }
    return (seq_t)sum; /* the result is guaranteed to be non‑negative */
}
