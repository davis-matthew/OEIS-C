/*
 * A332130  a(n) = (10^(2n+1)-1)/3 - 3*10^n.
 * https://oeis.org/A332130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 111*a(n-1) - 1110*a(n-2) + 1000*a(n-3)
   with a(0)=0, a(1)=303, a(2)=33033 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 303;
    if (n == 2) return 33033;

    seq_t a0 = 0;      /* a(n-3) */
    seq_t a1 = 303;    /* a(n-2) */
    seq_t a2 = 33033;  /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t tmp = (seq_t)111 * a2 + (seq_t)1000 * a0;
        seq_t a3 = tmp - (seq_t)1110 * a1;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
