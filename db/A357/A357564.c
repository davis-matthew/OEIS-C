/*
 * A357564  a(n) = n - 2*b(b(n)) for n >= 2, where b(n) = A006165(n).
 * https://oeis.org/A357564
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 2*b(b(n)), where b(n) = ⌊n φ⌋ (φ is the golden
 *            ratio). The sequence has the following closed form (taken from
 *            the OEIS entry): For n ≥ 5 let p = 2^k be the largest power of
 *            two satisfying 5p ≤ n < 10p. Then a(n) = p + (n − 5p) if n ≤ 8p
 *            (ascending part), a(n) = 7p − (n − 5p) if n > 8p...
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the offset 2…4 explicitly */
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)2;

    /* n ≥ 5.  Find the largest power of two p with 5p ≤ n < 10p. */
    idx_t p = 1;
    while (p <= n / 10)         /* for n/10 we avoid multiplication overflow */
        p <<= 1;                /* equivalent to p *= 2 */

    /* distance from the left end of the 5p‑wide block */
    idx_t j = n - 5 * p;

    /* compute a(n) using the piecewise linear formulas */
    if (j <= 3 * p)             /* ascending part: a = p + j */
        return (seq_t)(p + j);
    else                        /* descending part: a = 7p - j */
        return (seq_t)(7 * p - j);
}
