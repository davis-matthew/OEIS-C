/*
 * A319873  a(n) = 9*8*7*6*5*4*3*2*1 + 18*17*16*15*14*13*12*11*10 + ... + (up
 *          to the n-th term).
 * https://oeis.org/A319873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{m=1}^{⌊n/9⌋} Π_{i=0}^{8} (9m-i) + Sum_{j=1}^{8} [
 *            (n-j) mod 9 = 0 ] * Π_{i=1}^{j} (n - i - j + 10)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 17, u64 n <= 134, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A319873; k = 9. */
seq_t a(idx_t n)
{
    const int k = 9;                 /* block size */
    seq_t sum = 0;

    /* Sum over complete 9‑length blocks (multiples of k). */
    for (idx_t m = k; m <= n; m += k)
    {
        seq_t term = 1;
        for (int i = 0; i < k; ++i)
            term *= (seq_t)(m - i);   /* (m)(m-1)...(m-k+1) */

        sum += term;
    }

    /* Add the incomplete blocks that contribute partially. */
    for (int j = 1; j < k; ++j)
    {
        if (n >= (idx_t)j && ((n - j) % k) == 0)
        {
            seq_t term = 1;
            for (int i = 1; i <= j; ++i)
                term *= (seq_t)(n - i - j + k + 1); /* (n-j+9)...(n-2j+10) */

            sum += term;
        }
    }

    return sum;
}
