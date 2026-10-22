/*
 * A358504  Number of genetic relatives of a person M in a genealogical tree
 *          extending back n generations and where everyone has 3 children
 *          down to the generation of M.
 * https://oeis.org/A358504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9*a(n-1) - 20*a(n-2) + 12*a(n-3); a(0)=1, a(1)=5, a(2)=25
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)25;

    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)5;   /* a(n-2) */
    seq_t a2 = (seq_t)25;  /* a(n-1) */
    seq_t an;              /* a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)9 * a2 - (seq_t)20 * a1 + (seq_t)12 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
