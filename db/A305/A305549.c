/*
 * A305549  Crystal ball sequence for the lattice C_6.
 * https://oeis.org/A305549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (128*n^6 + 384*n^5 + 800*n^4 + 960*n^3 + 692*n^2 + 276*n
 *            + 45)/45
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 33, u64 n <= 1365,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for the intermediate calculations so that all
       intermediate values stay below 2^128 and avoid overflow on 64‑bit builds. */
    unsigned __int128 sn = (unsigned __int128)n;
    unsigned __int128 v = 128;            /* coefficient of n^6 */
    v = v * sn + 384;                     /* → 128*n + 384  (now 128*n^2 + 384*n + 800) after next steps */
    v = v * sn + 800;
    v = v * sn + 960;
    v = v * sn + 692;
    v = v * sn + 276;
    v = v * sn + 45;                      /* complete polynomial value */
    v /= 45;                              /* final division */
    return (seq_t)v;
}
