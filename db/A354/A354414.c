/*
 * A354414  a(n) is the smallest positive integer which does not occur in any
 *          Lucas sequence in which the first term is at most n and the second
 *          term is at most the first term.
 * https://oeis.org/A354414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) * F(k+2) - F(k) * F(k+1) where k is the largest
 *            index with F(k) <= n < F(k+1) (F(i) is the i-th Fibonacci
 *            number, F(0)=0, F(1)=1)
 * Range:     u8 n <= 14, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute Fibonacci numbers until the next one exceeds n */
    seq_t Fk   = 0;   /* F(0) */
    seq_t Fk1  = 1;   /* F(1) */
    idx_t  k   = 0;   /* current index k */

    while (Fk1 <= n) {
        seq_t next = Fk + Fk1;  /* F(k+2) */
        Fk  = Fk1;              /* move to F(k+1) */
        Fk1 = next;             /* move to F(k+2) */
        ++k;
    }
    /* Now Fk = F(k) <= n < F(k+1)=Fk1 */

    seq_t Fk2 = Fk + Fk1;        /* F(k+2) */

    /* (n+1) * F(k+2) - F(k) * F(k+1) */
    seq_t result = (seq_t)(n + 1) * Fk2 - Fk * Fk1;
    return result;
}
