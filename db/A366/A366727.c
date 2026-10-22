/*
 * A366727  2-tone chromatic number of a maximal outerplanar graph with
 *          maximum degree n.
 * https://oeis.org/A366727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(sqrt(2*n + 1/4) + 5/2) for n > 6. For 1 ≤ n ≤ 6:
 *            4, 6, 7, 7, 7, 7 respectively
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 128‑bit unsigned integer.
   Returned value is the floor of the true square root. */
static inline uint64_t isqrt_u128(unsigned __int128 x)
{
    uint64_t low = 0, high = (uint64_t)1 << 34;           /* sqrt(2^68) < 2^34. */
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x)  low = mid + 1;
        else         high = mid - 1;
    }
    return high;  /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    /* Handle the initial terms explicitly. */
    if (n <= 6) {
        static const seq_t init[] = {4, 6, 7, 7, 7, 7};
        return init[(unsigned)n - 1];
    }

    /* Compute ceil((sqrt(8n+1)+5)/2). */
    unsigned __int128 val = (unsigned __int128)n * (unsigned __int128)8 + 1;
    uint64_t s = isqrt_u128(val);                     /* floor sqrt of 8n+1 */
    unsigned __int128 sq = (unsigned __int128)s * s;   /* s^2 */

    seq_t ss   = (seq_t)s;          /* s as seq_t */
    seq_t tip  = ss + (seq_t)5;     /* s + 5 */

    /* If 8n+1 is a perfect square, use ceil((s+5)/2) == (s+5+1)/2. */
    /* Otherwise, ceil((s+5)/2) == floor((s+5)/2) + 1.          */
    return (sq == val) ? (tip + (seq_t)1) / 2
                       : (tip / 2) + (seq_t)1;
}
