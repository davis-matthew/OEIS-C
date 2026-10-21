/*
 * A299336  Expansion of 1 / ((1 - x)^7*(1 + x)^4).
 * https://oeis.org/A299336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 11*a(n-3) + 6*a(n-4) + 14*a(n-5) -
 *            14*a(n-6) - 6*a(n-7) + 11*a(n-8) - a(n-9) - 3*a(n-10) + a(n-11)
 *            for n > 10, implemented via a closed‐form closed form: a(n) =
 *            numerator / 23040 where, even n : numerator = 2*n^6 + 66*n^5 +
 *            860*n^4 + 5640*n^3 + 19568*n^2 + 33984*n + 230...
 * Range:     u8 n <= 6, u16 n <= 24, u32 n <= 186, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    /* use unsigned __int128 for intermediate computation
       all terms fit easily for n up to 10000. */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 n1 = nn;
    unsigned __int128 n2 = n1 * nn;
    unsigned __int128 n3 = n2 * nn;
    unsigned __int128 n4 = n3 * nn;
    unsigned __int128 n5 = n4 * nn;
    unsigned __int128 n6 = n5 * nn;
    unsigned __int128 num;

    if ((n & 1) == 0) {                /* n is even */
        num = 2 * n6 + 66 * n5 + 860 * n4 + 5640 * n3
              + 19568 * n2 + 33984 * n1 + 23040;
    } else {                           /* n is odd */
        num = 2 * n6 + 66 * n5 + 860 * n4 + 5580 * n3
              + 18578 * n2 + 28914 * n1 + 15120;
    }

    const unsigned __int128 denom = 23040;
    seq_t result = (seq_t)(num / denom);
    return result;
}
