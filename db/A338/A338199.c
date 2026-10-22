/*
 * A338199  a(n) = v(1 + F(4*n - 3)), where F(x) = (3*x + 1)/2^v(3*x + 1), x
 *          is any odd natural number, and v(y) is the 2-adic valuation of y.
 * https://oeis.org/A338199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v(1 + (12*n - 8)/2^v(12*n - 8))
 * Range:     u8 n <= 21, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where v(x) is the largest exponent such that 2^v divides x. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but guard against 0 */

    seq_t t = (seq_t)n * 12 - 8;    /* t = 12*n - 8 = 3*(4*n-3)+1          */
    seq_t u = t;
    unsigned v2 = 0;
    while ((u & 1) == 0)            /* strip factors of 2 from t          */
    {
        u >>= 1;
        v2++;
    }
    seq_t f = u;                    /* f = (12*n-8)/2^v2 = F(4*n-3)        */

    seq_t val = f + 1;               /* compute 1 + F(4*n-3)                */
    unsigned a_val = 0;
    while ((val & 1) == 0)           /* compute v(1 + F(4*n-3))             */
    {
        val >>= 1;
        a_val++;
    }
    return (seq_t)a_val;
}
