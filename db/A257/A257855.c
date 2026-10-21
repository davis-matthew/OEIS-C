/*
 * A257855  a(n) = 2*n^5 - floor(2^(1/5)*n)^5.
 * https://oeis.org/A257855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^5 - floor(2^(1/5)*n)^5
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for intermediate results. */
    unsigned __int128 sn = n;                 /* cast index to 128‑bit type */

    /* Compute n^5 and 2*n^5 */
    unsigned __int128 n5       = sn * sn * sn * sn * sn;
    unsigned __int128 two_n5   = n5 << 1;        /* 2 * n^5 */

    /* Integer fifth root of 2*n^5, i.e. floor(2^(1/5)*n). */
    unsigned __int128 low  = 0;
    unsigned __int128 high = sn << 1;           /* high = 2*n  (safe bound) */

    while (low < high)
    {
        /* mid = ceil((low+high)/2) */
        unsigned __int128 mid  = (low + high + 1) >> 1;
        unsigned __int128 mid5 = mid * mid * mid * mid * mid;

        if (mid5 <= two_n5)
            low = mid;          /* root is at least mid */
        else
            high = mid - 1;     /* root is less than mid */
    }

    /* low now equals floor((2*n^5)^(1/5)) = floor(2^(1/5)*n) */
    unsigned __int128 low5   = low * low * low * low * low;
    unsigned __int128 result = two_n5 - low5;

    return (seq_t)result;
}
