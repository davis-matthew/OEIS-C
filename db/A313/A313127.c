/*
 * A313127  Coordination sequence Gal.5.69.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>=1, * a(n) = 4 + 50*floor((n-1)/10) + s[(n-1)%10]
 *            * where s={0,5,10,15,21,27,32,37,42,46,50}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    idx_t m = n - 1;               /* number of differences after the first */
    idx_t q = m / 10;              /* number of full periods */
    idx_t r = m % 10;              /* remaining steps in the period */

    /* cumulative sums of the period P = {5,5,5,6,6,5,5,5,4,4} */
    static const seq_t partial[11] = {
        0ULL,   /* r = 0  */
        5ULL,   /* r = 1  */
        10ULL,  /* r = 2  */
        15ULL,  /* r = 3  */
        21ULL,  /* r = 4  */
        27ULL,  /* r = 5  */
        32ULL,  /* r = 6  */
        37ULL,  /* r = 7  */
        42ULL,  /* r = 8  */
        46ULL,  /* r = 9  */
        50ULL   /* r =10  */
    };

    seq_t result = (seq_t)4;                        /* a(1) = 4 */
    result += (seq_t)q * (seq_t)50;                 /* add full periods */
    result += partial[r];                           /* add remainder */

    return result;
}
