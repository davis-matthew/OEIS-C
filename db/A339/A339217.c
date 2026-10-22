/*
 * A339217  a(n) = Sum_{k=1..n} floor((2*n-k)/k).
 * https://oeis.org/A339217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} floor((2*n - k)/k) = sum_{k=1..n}
 *            floor(2*n/k) - n
 * Range:     u8 n <= 36, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t sum = 0;
    seq_t N = (seq_t)n * 2;   /* 2*n */

    idx_t k = 1;
    while (k <= n)
    {
        seq_t q = N / (seq_t)k;          /* floor(2n/k) */
        seq_t r_seq = N / q;             /* largest k with the same quotient */
        idx_t r;
        if (r_seq > (seq_t)n)
            r = n;
        else
            r = (idx_t)r_seq;

        sum += q * (seq_t)(r - k + 1);
        k = r + 1;
    }

    return sum - (seq_t)n;
}
