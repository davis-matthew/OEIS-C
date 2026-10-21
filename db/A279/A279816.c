/*
 * A279816  Digital roots of tetrahedral numbers (A000292).
 * https://oeis.org/A279816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digital_root(T_n) where T_n = n*(n+1)*(n+2)/6
 * Range:     u8 n <= 10, u16 n <= 72, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Digital root: dr(x) = 0 if x==0 else 1 + (x-1)%9 */

/* Computes the sequence A279816: digital roots of tetrahedral numbers. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* offset 0: a(0) = 0 */
        return (seq_t)0;

    /* Compute T_n = n*(n+1)*(n+2)/6 using seq_t arithmetic. */
    seq_t nn = (seq_t)n;
    seq_t t = nn * (nn + (seq_t)1) * (nn + (seq_t)2) / (seq_t)6;

    /* Digital root, but keep it in seq_t. */
    /* For seq_t unsigned types: if t==0 gives 0, otherwise 1+((t-1)%9). */
    if (t == (seq_t)0)
        return (seq_t)0;
    return (seq_t)(1 + (t - (seq_t)1) % (seq_t)9);
}
