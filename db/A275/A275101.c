/*
 * A275101  Number of set partitions of [4*n] such that within each block the
 *          numbers of elements from all residue classes modulo n are equal
 *          for n>0, a(0)=1.
 * https://oeis.org/A275101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is defined by the linear recurrence * a(n) = 47*a(n-1) -
 *            718*a(n-2) + 4416*a(n-3) - 10656*a(n-4) + 6912*a(n-5) * for n ≥
 *            6. * a(5) = 47*a(4) - 718*a(3) + 4416*a(2) - 10656*a(1) +
 *            6912*a(0) + 14112. * Initial values: a(0)=1, a(1)=15, a(2)=131,
 *            a(3)=1613, a(4)=25097
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)15;
    if (n == 2) return (seq_t)131;
    if (n == 3) return (seq_t)1613;
    if (n == 4) return (seq_t)25097;

    /* Constants for the recurrence */
    const seq_t c1 = (seq_t)47;
    const seq_t c2 = (seq_t)718;
    const seq_t c3 = (seq_t)4416;
    const seq_t c4 = (seq_t)10656;
    const seq_t c5 = (seq_t)6912;
    const seq_t c5_5 = (seq_t)14112; /* term added only for n = 5 */

    /* Variables to hold the last five values */
    seq_t a0 = (seq_t)1;   /* a(n-5) */
    seq_t a1 = (seq_t)15;  /* a(n-4) */
    seq_t a2 = (seq_t)131; /* a(n-3) */
    seq_t a3 = (seq_t)1613;/* a(n-2) */
    seq_t a4 = (seq_t)25097;/* a(n-1) */

    seq_t next;
    for (idx_t i = 5; i <= n; ++i)
    {
        /* Only for n = 5 we add the constant term from the rational part */
        seq_t const_term = (i == 5) ? c5_5 : (seq_t)0;

        next = c1 * a4 - c2 * a3 + c3 * a2 - c4 * a1 + c5 * a0 + const_term;

        /* Shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
