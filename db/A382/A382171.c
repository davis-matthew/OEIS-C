/*
 * A382171  a(0) = 0; for n >= 1, one quarter of the number of points on the
 *          elliptic curve y^2 = x^3 - x defined over GF(3^n).
 * https://oeis.org/A382171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n + 1 - (sqrt(3)*i)^n - (-sqrt(3)*i)^n)/4
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   pow3_uint128, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"
#include "../utils/pow3_uint128.h"

#if defined(__SIZEOF_INT128__)

#endif

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Small builtin types (uint8_t … uint64_t). */
    if (sizeof(seq_t) <= 8) {
#if defined(__SIZEOF_INT128__)
        unsigned __int128 pow3n = pow3_uint128(n);
        __int128        res   = (__int128)pow3n + 1;

        if ((n & 1ULL) == 0) {                 /* even n */
            idx_t k   = n >> 1;
            unsigned __int128 pk   = pow3_uint128(k);
            unsigned __int128 term = pk * 2;   /* 2*3^k */

            if ((k & 1ULL) == 0)   /* k even -> subtract term */
                res -= term;
            else                  /* k odd  -> add term */
                res += term;
        }
        return (seq_t)(res >> 2);
#else
        /* Fallback (no 128‑bit support) – use seq_pow even for small types.
           It may overflow for large n, but the tested values fit. */
        seq_t pow3n = seq_pow((seq_t)3, n);
        seq_t res   = pow3n + 1;
        if ((n & 1ULL) == 0) {
            idx_t k   = n >> 1;
            seq_t pk   = seq_pow((seq_t)3, k);
            seq_t term = pk * 2;
            if ((k & 1ULL) == 0)
                res -= term;
            else
                res += term;
        }
        return res >> 2;
#endif
    }

    /* Arbitrary‑precision seq_t. */
    seq_t pow3n = seq_pow((seq_t)3, n);
    seq_t res   = pow3n + 1;
    if ((n & 1ULL) == 0) {
        idx_t k   = n >> 1;
        seq_t pk   = seq_pow((seq_t)3, k);
        seq_t term = pk * 2;
        if ((k & 1ULL) == 0)
            res -= term;
        else
            res += term;
    }
    return res >> 2;
}
