/*
 * A387778  Lower (1/2)-midsequence of (F(2n)) and (F(2n+1)), where F=A000045
 *          (Fibonacci numbers); see Comments.
 * https://oeis.org/A387778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((F(2*n) + F(2*n+1))/2) where F is Fibonacci
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the fast‑doubling method to compute F(k) and F(k+1). */

static void fib_pair_local(idx_t n, seq_t *fn, seq_t *fn1)
{
    seq_t a = 0;            /* F(0)  */
    seq_t b = 1;            /* F(1)  */
    /* iterate over the bits of n from most significant to least */
    for (int i = 63; i >= 0; --i) {
        int bit = (n >> i) & 1;
        /* fast‑doubling formulas:
           c = F(2k)   = F(k) * (2*F(k+1) - F(k))
           d = F(2k+1) = F(k)^2 + F(k+1)^2
        */
        seq_t c = a * ((b << 1) - a);
        seq_t d = a*a + b*b;
        if (bit == 0) {
            a = c;
            b = d;
        } else {
            a = d;
            b = c + d;
        }
    }
    *fn  = a;   /* F(n)   */
    *fn1 = b;   /* F(n+1) */
}

seq_t a(idx_t n)
{
    seq_t f2n, f2n1;
    fib_pair_local(2 * n, &f2n, &f2n1);   /* compute F(2n) and F(2n+1) */
    return (f2n + f2n1) / 2;              /* floor division by 2 */
}
