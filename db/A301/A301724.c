/*
 * A301724  Coordination sequence for node of type V1 in "kra" 2-D tiling (or
 *          net).
 * https://oeis.org/A301724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; For n>=1: a(n)=6 + floor((n-1)/10)*54 +
 *            prefix[(n-1)%10], where prefix = {0,4,10,17,21,25,32,38,42,48}
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* Prefix sums of the 10‑term difference period (starting at n=2). */
    static const seq_t period_prefix[10] = {
        0,  /* (n-1)%10 == 0   → no extra difference */
        4,  /* first difference 4  (n=2)  */
        10, /* 4+6              (n=3)  */
        17, /* +7              (n=4)  */
        21, /* +4              (n=5)  */
        25, /* +4              (n=6)  */
        32, /* +7              (n=7)  */
        38, /* +6              (n=8)  */
        42, /* +4              (n=9)  */
        48  /* +6              (n=10) */
    };

    idx_t m = n - 1;                 /* number of differences from n=1 to n */
    idx_t periods = m / 10;          /* count of complete periods */
    idx_t rem     = m % 10;          /* remainder in the current period */

    seq_t result = 6;                /* value a(1) */
    /* add contributions of full periods (each sums to 54) */
    result += (seq_t)periods * 54;
    /* add the remaining part of the current period */
    result += period_prefix[rem];
    return result;
}
