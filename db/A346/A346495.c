/*
 * A346495  Smallest a(n) so that division by n can be performed by floor(x/n)
 *          = floor(x*a(n)/2^A346496(n)) for any 0 <= x < 2^32.
 * https://oeis.org/A346495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini
 * Formula:   a(n) = 1 if n is a power of two; * otherwise a(n) is the
 *            smallest integer m such that * floor(x/m) = floor(x*a(n)/2^b)
 *            for all 0 ≤ x < 2^32, * where b is the minimal exponent
 *            satisfying * 2^b > (2^32 - (2^32 mod n) - 1) * (n-1 - ((2^b-1)
 *            mod n)). * This implementation uses 64‑bit arithmetic for t...
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 6, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0, but return 0 */

    /* power of two? */
    if ((n & (n - 1)) == 0)
        return (seq_t)1;

    /* 2^32 and remainder of 2^32 modulo n */
    const uint64_t two32 = (uint64_t)1 << 32;
    const uint64_t n64   = (uint64_t)n;
    const uint64_t rem2  = two32 % n64;
    const uint64_t n_c   = two32 - rem2 - 1;

    /* search for minimal b */
    uint64_t pow2_b = 1;   /* 2^b */
    uint64_t b = 0;
    while (1) {
        uint64_t z   = (pow2_b - 1) % n64;            /* (2^b-1) mod n */
        uint64_t val = n_c * ((n64 - 1) - z);        /* n_c * (n-1 - z) */
        if (pow2_b > val)
            break;
        b++;
        pow2_b <<= 1;
    }

    /* compute a(n) */
    uint64_t z   = (pow2_b - 1) % n64;            /* (2^b-1) mod n */
    uint64_t a_val = (pow2_b + n64 - 1 - z) / n64;

    return (seq_t)a_val;
}
