/*
 * A313826  Coordination sequence Gal.4.78.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n for even n>0; a(n)=1 for n=0; a(n) = 5*n + 1 if
 *            (n-1)/2 ≡ 1 (mod 4); a(n) = 5*n - 1 if (n-1)/2 ≡ 2 (mod 4);
 *            otherwise a(n) = 5*n
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Even indices (except 0) */
    if ((n & 1) == 0)
        return (seq_t)n * (seq_t)5;

    /* Odd indices */
    idx_t m = (n - 1) >> 1;           /* (n-1)/2  */
    uint64_t r = m & 3ULL;           /* m mod 4  */
    seq_t val = (seq_t)n * (seq_t)5; /* 5*n     */

    switch (r) {
        case 1:
            return val + (seq_t)1;   /* add 1 */
        case 2:
            return val - (seq_t)1;   /* subtract 1 */
        default:
            return val;              /* no adjustment */
    }
}
