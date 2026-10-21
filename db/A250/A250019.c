/*
 * A250019  Number of length 5+5 0..n arrays with no six consecutive terms
 *          having the maximum of any three terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A250019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^10 - (41/105)*n^9 + (883/168)*n^8 + (3/70)*n^7 +
 *            (593/60)*n^6 + (7/8)*n^4 + (1741/210)*n^3 - (491/70)*n^2 +
 *            (72/35)*n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit unsigned integer for all intermediate calculations.
       The result always fits in seq_t because the final values are
       guaranteed to be representable in the chosen seq_t. */
    typedef unsigned __int128 u128;
    u128 pw[11];
    pw[0] = (u128)1;
    for (idx_t i = 1; i <= 10; ++i)
        pw[i] = pw[i - 1] * (u128)n;

    /* Integer polynomial Q(n) = 840*a(n)
       Q(n) = 840*n^10 - 328*n^9 + 4415*n^8 + 36*n^7 + 8302*n^6
              + 735*n^4 + 6964*n^3 - 5892*n^2 + 1728*n          */
    const u128 c10 = (u128)840;
    const u128 c9  = (u128)328;   /* subtract */
    const u128 c8  = (u128)4415;
    const u128 c7  = (u128)36;
    const u128 c6  = (u128)8302;
    const u128 c4  = (u128)735;
    const u128 c3  = (u128)6964;
    const u128 c2  = (u128)5892;   /* subtract */
    const u128 c1  = (u128)1728;

    u128 sum = 0;
    sum += c10 * pw[10];
    sum -= c9  * pw[9];
    sum += c8  * pw[8];
    sum += c7  * pw[7];
    sum += c6  * pw[6];
    sum += c4  * pw[4];
    sum += c3  * pw[3];
    sum -= c2  * pw[2];
    sum += c1  * pw[1];

    /* divide by 840 to obtain a(n) */
    u128 result128 = sum / (u128)840;
    return (seq_t)result128;
}
