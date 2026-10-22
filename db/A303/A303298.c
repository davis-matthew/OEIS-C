/*
 * A303298  Generalized 21-gonal (or icosihenagonal) numbers: m*(19*m - 17)/2
 *          with m = 0, +1, -1, +2, -2, +3, -3, ...
 * https://oeis.org/A303298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = m*(19*m - 17)/2 with m = 0, +1, -1, +2, -2, +3, -3, ...
 *            for n = 0,1,2,
 * Range:     u8 n <= 9, u16 n <= 165, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  m = ceil(n/2) with a sign: (+) for odd n, (−) for even n (except n=0) */
seq_t a(idx_t n)
{
    /* Compute m using a signed 128‑bit type to keep sign information and avoid overflow */
    __int128 m = (__int128)((n + 1ULL) / 2ULL);           /* |m| = ceil(n/2) */
    if ((n & 1ULL) == 0ULL) m = -m;                      /* even n → negative m, n=0 gives 0 */

    /* Compute a(n) = m*(19*m - 17)/2 */
    __int128 tmp = (__int128)19 * m - 17;
    __int128 res = m * tmp / 2;

    return (seq_t)res;     /* cast to the build‑specific integer type */
}
