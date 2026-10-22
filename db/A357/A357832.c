/*
 * A357832  a(n) = Sum_{k=0..floor((n-1)/3)} 2^k * |Stirling1(n,3*k+1)|.
 * https://oeis.org/A357832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/3)} 2^k * |Stirling1(n,3*k+1)|
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* A(n)  = a(n)            */
    /* B(n)  = Sum_{k} 2^k * |Stirling1(n,3*k)   |  (residue 0) */
    /* C(n)  = Sum_{k>=1} 2^k * |Stirling1(n,3*k-1)| (residue 2) */

    seq_t A = 0;   /* A(0) = 0 */
    seq_t B = 1;   /* B(0) = 1 (k=0 term |Stirling1(0,0)| = 1) */
    seq_t C = 0;   /* C(0) = 0 */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* multiplier is (i-1) because we are stepping from n=i-1 to n=i */
        seq_t mul = (seq_t)(i - 1);

        seq_t newA = mul * A + B;          /* A(i) = (i-1)*A(i-1) + B(i-1) */
        seq_t newB = mul * B + C;          /* B(i) = (i-1)*B(i-1) + C(i-1) */
        seq_t newC = mul * C + (seq_t)2 * A;/* C(i) = (i-1)*C(i-1) + 2*A(i-1) */

        A = newA;
        B = newB;
        C = newC;
    }

    return A;
}
