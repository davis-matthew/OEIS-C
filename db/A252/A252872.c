/*
 * A252872  Number of n X 4 nonnegative integer arrays with upper left 0 and
 *          lower right n+4-4 and value increasing by 0 or 1 with every step
 *          right or down.
 * https://oeis.org/A252872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5*n^8 + 140*n^7 + 1442*n^6 + 7616*n^5 + 22925*n^4 +
 *            46340*n^3 - 24372*n^2 - 13776*n + 80640) / 40320 (n >= 1)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 45, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1, return 0 */

    /* Use 128‑bit signed arithmetic for the intermediate
       accumulation so that all intermediate values fit
       for n up to the expected range (≈10⁴). */
    __int128_t nn = n;
    __int128_t t  = 5;            /* leading coefficient */

    t = t * nn + 140;
    t = t * nn + 1442;
    t = t * nn + 7616;
    t = t * nn + 22925;
    t = t * nn + 46340;
    t = t * nn - 24372;
    t = t * nn - 13776;
    t = t * nn + 80640;
    t /= 40320;                 /* exact division */

    return (seq_t)t;
}
