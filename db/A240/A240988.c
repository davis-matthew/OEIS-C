/*
 * A240988  Denominators of the (reduced) rationals (((n-1)!!)/(n!! * 2^((1 +
 *          (-1)^n)/2)))^((-1)^n), where n is a positive integer.
 * https://oeis.org/A240988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  
 *   a(n) = 2^(k + v₂(k!))         for odd n = 2k+1
 *   a(n) = 2^(k+1 + v₂(k!))       for even n = 2k
 *   where v₂(m!) is the exponent of 2 in m!.
 */

#include <stdint.h>
#include "../utils/pow2.h"

static inline idx_t popcnt(idx_t x)
{
#ifdef __GNUC__
    return __builtin_popcountll(x);
#else
    idx_t c = 0;
    while (x) { x &= x - 1; ++c; }
    return c;
#endif
}

seq_t a(idx_t n)
{
    /* n is ≥ 1; values for n = 0 are not defined by OEIS offset */
    idx_t k, e;
    if ((n & 1u) == 0u)             /* even n = 2k */
    {
        k  = n >> 1u;
        idx_t pc = popcnt(k);        /* number of 1‑bits in k */
        e   = k + 1u + (k - pc);     /* k + 1 + v₂(k!)  */
    }
    else                            /* odd n = 2k+1 */
    {
        k  = (n - 1u) >> 1u;
        idx_t pc = popcnt(k);
        e   = k + (k - pc);          /* k + v₂(k!)  */
    }
    return pow2(e);                  /* a(n) = 2^e */
}
