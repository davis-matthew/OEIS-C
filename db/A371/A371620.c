/*
 * A371620  Fractal sequence based on a heptagon: a(n) = number of unobscured
 *          heptagons remaining after completing step n.
 * https://oeis.org/A371620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 3*a(n-1) + 14*a(n-2) + 10*a(n-3) (for n >= 4) with
 *            initial values: a(1)=1, a(2)=1, a(3)=8
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)8;

    /* Iterate using recurrence */
    seq_t a0 = (seq_t)1;  /* a(n-3)  */
    seq_t a1 = (seq_t)1;  /* a(n-2)  */
    seq_t a2 = (seq_t)8;  /* a(n-1)  */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = a2 * (seq_t)3 + a1 * (seq_t)14 + a0 * (seq_t)10;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
