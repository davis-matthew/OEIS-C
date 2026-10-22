/*
 * A335092  a(n) = ((2*n+1)!!)^4 * (Sum_{k=1..n} 1/(2*k+1)^4).
 * https://oeis.org/A335092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2*n-1)^4 + (2*n+1)^4) * a(n-1) - (2*n-1)^8 * a(n-2) for
 *            n>1 a(0) = 0, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prev2 = 0; /* a(0) */
    seq_t prev1 = 1; /* a(1) */

    if (n == 0)
        return prev2;
    if (n == 1)
        return prev1;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t m  = (seq_t)2 * (seq_t)i; // 2*i
        seq_t p  = m - 1;               // 2*i-1
        seq_t q  = m + 1;               // 2*i+1

        seq_t p2 = p * p;
        seq_t p4 = p2 * p2;
        seq_t p8 = p4 * p4;

        seq_t q2 = q * q;
        seq_t q4 = q2 * q2;

        seq_t sum = p4 + q4;
        seq_t cur = sum * prev1 - p8 * prev2;

        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
