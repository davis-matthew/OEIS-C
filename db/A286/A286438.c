/*
 * A286438  Number of ways to tile an n X n X n triangular area with three 2 X
 *          2 X 2 triangular tiles and an appropriate number (= n^2-12) of 1 X
 *          1 X 1 tiles.
 * https://oeis.org/A286438
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 - 9*n^5 + 6*n^4 + 153*n^3 - 361*n^2 - 564*n +
 *            1848)/6 for n ≥ 4; a(3) = 0. Operations are performed in
 *            unsigned __int128 to avoid overflow, then cast to seq_t
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 55, u64 n <= 2192,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t if needed */

seq_t a(idx_t n)
{
    /* For the offset 3, a(3) = 0.  Values below are undefined but
       returning 0 is safe for this sequence. */
    if (n <= 3)
        return 0;

    /* Compute in 128‑bit arithmetic to guarantee intermediate precision. */
    unsigned __int128 nn = n;
    unsigned __int128 n2 = nn * nn;
    unsigned __int128 n3 = n2 * nn;
    unsigned __int128 n4 = n3 * nn;
    unsigned __int128 n5 = n4 * nn;
    unsigned __int128 n6 = n5 * nn;

    /* Separate positive and negative contributions to avoid
       temporary wrap‑around in unsigned arithmetic. */
    unsigned __int128 pos =
        n6
        + (unsigned __int128)6 * n4
        + (unsigned __int128)153 * n3
        + (unsigned __int128)1848;

    unsigned __int128 neg =
        (unsigned __int128)9 * n5
        + (unsigned __int128)361 * n2
        + (unsigned __int128)564 * nn;

    unsigned __int128 num = pos - neg;
    unsigned __int128 result = num / (unsigned __int128)6;

    return (seq_t)result;
}
