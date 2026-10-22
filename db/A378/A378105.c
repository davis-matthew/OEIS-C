/*
 * A378105  Number of non-colorable strings of length n considered by the
 *          alternating coloring function.
 * https://oeis.org/A378105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n + 8)/6 for even n>=2 a(n) = (2^n + 16)/6 for odd
 *            n>=3 a(0)=1, a(1)=2, a(2)=2 Computation uses only integer
 *            arithmetic and works for arbitrary precision seq_t. For C builds
 *            without __int128 we avoid overflow by iteratively building
 *            floor(2^(n-1)/3) and adding the small offset
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the exceptional small indices */
    if (n == 0)        return (seq_t)1;
    if (n == 1 || n == 2) return (seq_t)2;

    /* Compute q = floor(2^(n-1)/3) iteratively.
       r holds 2^(k) mod 3 for the current power. */
    seq_t q = 0;
    int r = 1;  /* 2^0 mod 3 = 1 */

    for (idx_t i = 1; i <= n - 1; ++i) {
        /* q = floor(2*2^(i-1)/3) = 2*q + floor( (2*r)/3 ) */
        q <<= 1;                /* multiply by 2 */
        if (r == 2)             /* floor(4/3) == 1 */
            q += 1;             /* add the carry if remainder was 2 */
        /* update remainder for next step: r = (2*r) % 3 */
        r = (r << 1) % 3;
    }

    /* Now q = floor(2^(n-1)/3).  Add the constant depending on parity of n. */
    seq_t offset = (n & 1) ? (seq_t)3 : (seq_t)2;  /* 3 if n odd, 2 if n even */
    return q + offset;
}
