/*
 * A290361  Number of 7-leaf rooted trees with n levels.
 * https://oeis.org/A290361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (61*n^6+69*n^5+145*n^4+195*n^3+154*n^2+96*n)/720
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 60, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (0-indexed).  The function uses 128‑bit arithmetic to avoid overflow
   for all n that fit in a 64‑bit integer. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case directly. The formula also gives 0 for n=0. */
    if (n == 0) {
        return 0;
    }

    /* All intermediate calculations are performed in unsigned __int128
       to keep them within range for n up to at least 10,000. */
    unsigned __int128 nn  = (unsigned __int128)n;
    unsigned __int128 n2  = nn * nn;            /* n^2   */
    unsigned __int128 n3  = n2 * nn;            /* n^3   */
    unsigned __int128 n4  = n3 * nn;            /* n^4   */
    unsigned __int128 n5  = n4 * nn;            /* n^5   */
    unsigned __int128 n6  = n5 * nn;            /* n^6   */

    unsigned __int128 num =
          (unsigned __int128)61 * n6
        + (unsigned __int128)69 * n5
        + (unsigned __int128)145 * n4
        + (unsigned __int128)195 * n3
        + (unsigned __int128)154 * n2
        + (unsigned __int128)96  * nn;

    unsigned __int128 res = num / (unsigned __int128)720;  /* divide by 6! = 720 */

    return (seq_t)res;
}
