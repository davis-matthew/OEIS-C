/*
 * A336286  The hafnian of a symmetric Toeplitz matrix of order 2*n, n>=2 with
 *          the first row (0,1,2,...,2,0); a(0)=a(1)=1.
 * https://oeis.org/A336286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5, 57, 859 for n=2,3,4 and a(n+1) = (4n+4)*a(n) -
 *            (8n-13)*a(n-1) - 2*a(n-2) for n>=4. Initial values: a(0)=1,
 *            a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 5;
    if (n == 3) return 57;
    if (n == 4) return 859;

    seq_t prev2 = 5;   /* a(2) */
    seq_t prev1 = 57;  /* a(3) */
    seq_t curr  = 859; /* a(4) */

    idx_t i;
    for (i = 4; i < n; ++i) {
        seq_t coeff1 = (seq_t)(4 * i + 4);
        seq_t coeff2 = (seq_t)(8 * i - 13);
        seq_t term1  = coeff1 * curr;
        seq_t term2  = coeff2 * prev1;
        seq_t term3  = (seq_t)2 * prev2;
        seq_t next   = term1 - term2 - term3;

        prev2 = prev1;
        prev1 = curr;
        curr  = next;
    }
    return curr;
}
