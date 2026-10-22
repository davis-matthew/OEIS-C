/*
 * A333591  Alternate multiplying and adding Fibonacci numbers: a(0) = F(0) *
 *          F(1), for n >= 0, a(2n+1) = a(2n) + F(2n+2), a(2n+2) = a(2n+1) *
 *          F(2n+3).
 * https://oeis.org/A333591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = F(0)*F(1); for n ≥ 0, a(2n+1) = a(2n) + F(2n+2); a(2n+2)
 *            = a(2n+1) * F(2n+3); (F is the Fibonacci sequence, F(0)=0,
 *            F(1)=1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 13, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = F(0)*F(1) = 0*1 = 0 */
    seq_t a_even = (seq_t)0 * (seq_t)1;

    if (n == 0)
        return a_even;

    idx_t i = 0;
    /* Maintain F(2i) and F(2i+1). Initially 0 and 1. */
    seq_t f_k   = (seq_t)0;   /* F(2i)   */
    seq_t f_k1  = (seq_t)1;   /* F(2i+1) */

    while (1) {
        /* Compute F(2i+2) and F(2i+3) without extra memory. */
        seq_t f2 = f_k + f_k1;     /* F(2i+2) */
        seq_t f3 = f2 + f_k1;      /* F(2i+3) = F(2i+2) + F(2i+1) */

        /* a(2i+1) = a(2i) + F(2i+2) */
        seq_t a_odd = a_even + f2;
        if (n == 2 * i + 1)
            return a_odd;

        /* a(2i+2) = a(2i+1) * F(2i+3) */
        seq_t a_even_next = a_odd * f3;
        if (n == 2 * i + 2)
            return a_even_next;

        /* Advance to next two indices. */
        a_even = a_even_next;
        f_k    = f2;
        f_k1   = f3;
        i++;
    }
}
