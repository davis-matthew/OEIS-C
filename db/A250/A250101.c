/*
 * A250101  Expansion of (18*x+86*x^2)/(1-11*x^2-150*x^3).
 * https://oeis.org/A250101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=18, a(2)=86,
   a(n)=11*a(n-2)+150*a(n-3) for n≥3 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 18;
    if (n == 2) return 86;

    seq_t a_nm3 = 0;   /* a(0) */
    seq_t a_nm2 = 18;  /* a(1) */
    seq_t a_nm1 = 86;  /* a(2) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        next = 11 * a_nm2 + 150 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }
    return a_nm1;
}
