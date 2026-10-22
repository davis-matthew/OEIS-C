/*
 * A325695  Number of length-3 strict integer partitions of n such that the
 *          largest part is not the sum of the other two.
 * https://oeis.org/A325695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n^2-6n+12)/12) - (n even ? floor((n/2-1)/2) : 0)
 * Range:     u8 n <= 58, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts length‑3 strict partitions of n without the largest part being the sum of the other two. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 7)                /* no valid partitions for n < 7 */
        return 0;

    /* Use 64‑bit arithmetic for the intermediate calculation to avoid overflow.
       The final value fits in seq_t for the expected ranges. */
    unsigned long long nn = (unsigned long long)n;
    unsigned long long num = nn * nn - 6 * nn + 12;      /* n^2 - 6n + 12 */
    seq_t D = (seq_t)(num / 12);                         /* total distinct 3‑part partitions */

    seq_t E = 0;                                         /* partitions with largest = sum of others */
    if ((n & 1) == 0) {                                 /* only possible when n is even    */
        seq_t m = (seq_t)(n / 2);                        /* m = n/2                        */
        E = (m - 1) / 2;                                /* floor((m-1)/2)                 */
    }

    return D - E;                                       /* desired count                   */
}
