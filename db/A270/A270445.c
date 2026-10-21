/*
 * A270445  Expansion of 2*x*(1+4*x) / (1-12*x+16*x^2).
 * https://oeis.org/A270445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A270445
   Recurrence:  a(1)=2, a(2)=32, a(n)=12*a(n-1)-16*a(n-2)  (n>2)
   (equivalently a(n) = (1+√5)^(2n-1)+(1-√5)^(2n-1)) */

seq_t a(idx_t n)
{
    /* offset is 1; for n<1 we return 0 (undefined) */
    if (n < 1) return (seq_t)0;

    /* base cases */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)32;

    /* iterative computation of the linear recurrence */
    seq_t a1 = (seq_t)2;   /* a(1) */
    seq_t a2 = (seq_t)32;  /* a(2) */
    seq_t an = 0;

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)12 * a2 - (seq_t)16 * a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
