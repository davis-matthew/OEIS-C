/*
 * A319571  The stripe enumeration of N X N where N = {0, 1, 2, ...}, also
 *          called boustrophedonic Cantor enumeration. Terms are interleaved x
 *          and y coordinates.
 * https://oeis.org/A319571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes A319571: interleaved coordinates of the boustrophedonic Cantor enumeration.
    a(n) returns the n‑th coordinate (x for even n, y for odd n).  */
seq_t a(idx_t n)
{
    /* Pair index (zero‑based). */
    idx_t k = n >> 1;

    /* Find diagonal s such that k lies in it:
       we want the smallest s with (s+1)(s+2)/2 > k.
       s always fits in idx_t for all 0 <= n < UINT64_MAX.          */
    idx_t s = 0;
    while (( (__int128)(s + 1) * (s + 2) / 2 ) <= k) {
        ++s;
    }

    /* Number of pairs before diagonal s. */
    idx_t prev_pairs = ( (__int128)s * (s + 1) / 2 );

    /* Position within the diagonal. */
    idx_t offset = k - prev_pairs;

    /* Determine (x, y) for diagonal s.   */
    idx_t x, y;
    if ((s & 1ULL) == 0) {            /* even s : descending x  */
        x = s - offset;
        y = offset;
    } else {                          /* odd s  : ascending x  */
        x = offset;
        y = s - offset;
    }

    /* Return the coordinate that corresponds to the parity of n. */
    return (n & 1ULL) ? (seq_t)y : (seq_t)x;
}
