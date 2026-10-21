/*
 * A256821  Number of length n+6 0..1 arrays with at most two downsteps in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A256821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (n^5 + 25*n^4 + 1665*n^3 - 3505*n^2 + 23534*n - 30360)/120
 *   for n >= 5.
 *   a(1) = 128, a(2) = 256, a(3) = 512, a(4) = 1024.
 */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* values for the first four indices (offset = 1) */
    static const seq_t first[5] = {0, 128, 256, 512, 1024};
    if (n < 5)
        return first[n];          /* n == 0 returns 0 as a default */

    /* compute the polynomial for n >= 5 using 128‑bit arithmetic to avoid overflow */
    __int128 N = n;                /* promote index to 128–bit type */

    __int128 num = N * N * N * N * N;        /* n^5 */
    num += 25 * N * N * N * N;              /* +25*n^4 */
    num += 1665 * N * N * N;                /* +1665*n^3 */
    num -= 3505 * N * N;                    /* -3505*n^2 */
    num += 23534 * N;                       /* +23534*n */
    num -= 30360;                           /* -30360 */

    return (seq_t)(num / 120);              /* divide by 120 to get the exact integer result */
}
