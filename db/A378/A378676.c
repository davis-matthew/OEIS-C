/*
 * A378676  a(n) = J(n) * J(n+2) where J(n) = Jacobsthal(n) = A001045(n).
 * https://oeis.org/A378676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = J(n) * J(n+2) Recurrence: a(0)=0, a(1)=3, a(2)=5, a(n) =
 *            3*a(n-1) + 6*a(n-2) - 8*a(n-3) for n>2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first three terms explicitly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;

    /* Iterate using the linear recurrence. */
    seq_t a0 = (seq_t)0;   /* a(n-3)  */
    seq_t a1 = (seq_t)3;   /* a(n-2)  */
    seq_t a2 = (seq_t)5;   /* a(n-1)  */
    seq_t an = 0;
    for (idx_t i = 3; i <= n; ++i) {
        an = (3 * a2) + (6 * a1) - (8 * a0);
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
