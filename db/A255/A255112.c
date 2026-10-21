/*
 * A255112  Number of length n+5 0..2 arrays with at most one downstep in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^5 + 40*n^4 + 575*n^3 + 17780*n^2 + 46404*n + 13320) /
 *            120 for n > 3; a(1)=729, a(2)=1791, a(3)=2907
 * Range:     u8 none, u16 n <= 14, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Small‑n exact values from the sequence data */
    static const seq_t small[4] = {
        0,          /* dummy for a(0), unused */
        729ULL,     /* a(1) */
        1791ULL,    /* a(2) */
        2907ULL     /* a(3) */
    };

    if (n <= 3) {
        return small[n];
    }

    /* Compute the polynomial exactly using 128‑bit arithmetic
       to avoid overflow when n can be as large as ~10,000. */
    unsigned __int128 t   = (unsigned __int128)n;
    unsigned __int128 n2  = t * t;          /* n^2   */
    unsigned __int128 n3  = n2 * t;         /* n^3   */
    unsigned __int128 n4  = n3 * t;         /* n^4   */
    unsigned __int128 n5  = n4 * t;         /* n^5   */

    unsigned __int128 num =
        n5
        + (unsigned __int128)40   * n4
        + (unsigned __int128)575  * n3
        + (unsigned __int128)17780 * n2
        + (unsigned __int128)46404 * t
        + (unsigned __int128)13320;

    return (seq_t)(num / 120);
}
