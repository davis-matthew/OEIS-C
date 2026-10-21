/*
 * A254472  Sixth partial sums of sixth powers (A001014).
 * https://oeis.org/A254472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            n*(n+1)*(n+2)*(n+3)^2*(n+4)*(n+5)*(n+6)*(-3+5*n+n^2)*(3+7*n+n^2)/665280
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 16, u64 n <= 120, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* undefined for n < 1, harmless */

    unsigned __int128 prod = 1;         /* 128‑bit intermediate */
    unsigned __int128 sn = (unsigned __int128)n;

    prod *= sn;                        /* n */
    prod *= sn + 1;                    /* n+1 */
    prod *= sn + 2;                    /* n+2 */

    unsigned __int128 n3 = sn + 3;      /* n+3 */
    prod *= n3;                        /* (n+3) */
    prod *= n3;                        /* (n+3)^2 */

    prod *= sn + 4;                    /* n+4 */
    prod *= sn + 5;                    /* n+5 */
    prod *= sn + 6;                    /* n+6 */

    /* (-3 + 5*n + n^2) and (3 + 7*n + n^2) */
    prod *= (sn*sn + sn*5 - (unsigned __int128)3);
    prod *= (sn*sn + sn*7 + (unsigned __int128)3);

    prod /= 665280;                    /* exact division */

    return (seq_t)prod;                /* truncate to seq_t if necessary */
}
