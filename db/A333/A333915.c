/*
 * A333915  Number of ways to represent n as a pyramidal number.
 * https://oeis.org/A333915
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (s, m) : s ≥ 3, m ≥ 2, Σ_{r=1}^m Pₛ(r) = n }
 * Range:     u8 n <= 21, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where Pₛ(r) = ((s-2) r² - (s-4) r)/2 is the r‑th s‑gonal number. */

seq_t a(idx_t n)
{
    /* a(n) is undefined for n < 4; we return 0 for safety. */
    if (n < 4) return (seq_t)0;

    seq_t count = 0;
    const seq_t twelve = 12;

    /* Loop over possible heights m of the pyramid.
       The minimal value for fixed m occurs at s = 3 (triangular numbers).
       For m ≥ 2 this minimal is tetrahedral: T(m) = m(m+1)(m+2)/6. */
    for (idx_t m = 2; ; ++m)
    {
        /* Compute tetrahedral(m). */
        seq_t tet = (seq_t)m * (m + 1) * (m + 2) / 6;
        if (tet > n) break;                /* further m will only increase tet */

        seq_t T = (seq_t)m * (m + 1);      /* m(m+1)          */
        seq_t numerator12n = twelve * (seq_t)n;
        if (T == 0 || (numerator12n % T) != 0) continue;  /* 12*n must be divisible by T */

        seq_t X = numerator12n / T;         /* X = (12 n) / (m(m+1)) */
        idx_t D = 2 * m + 1;                /* D = 2m + 1 (odd)   */

        seq_t denom = D - 3;                /* D-3 > 0 for m ≥ 2 */
        seq_t numerator = X + 2 * (seq_t)D - 12;   /* numerator for s */

        if ((numerator % denom) != 0) continue;    /* s must be integer */

        seq_t s = numerator / denom;
        if (s >= 3) ++count;                 /* valid representation */
    }

    return count;
}
