/*
 * A342362  Expansion of the o.g.f. (1 + 8*x + 10*x^2 + 8*x^3 + x^4)/((1 -
 *          x)^4*(1 + x)^2).
 * https://oeis.org/A342362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1)*(14*(n + 1)^2 - 3*(-1)^n + 1)/12
 * Range:     u8 n <= 5, u16 n <= 37, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 128‑bit intermediates avoid overflow for any supported seq_t type. */
    __int128 n1 = (__int128)n + 1;              /* n + 1 */
    __int128 expr = 14 * n1 * n1;               /* 14*(n+1)^2 */
    if ((n & 1) == 0) {                         /* even n */
        expr -= 2;                              /* 14*(n+1)^2 - 2 */
    } else {                                    /* odd n */
        expr += 4;                              /* 14*(n+1)^2 + 4 */
    }
    __int128 prod = n1 * expr;                  /* (n+1)*expr */
    prod /= 12;                                 /* exact division by 12 */
    return (seq_t)prod;
}
