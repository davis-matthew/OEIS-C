/*
 * A285982  a(n) = n! (mod n + 3).
 * https://oeis.org/A285982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! mod (n + 3)
 * Range:     u8 n <= 517, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_mod64.h"

seq_t a(idx_t n)
{
    /* The modulus is n + 3, which fits in a 64‑bit integer. */
    uint64_t mod = n + 3;
    uint64_t res = 1 % mod;          /* 0! = 1 */
    for (idx_t i = 1; i <= n; ++i) {
        /* Compute res = (res * i) mod mod using safe 64‑bit modular multiplication. */
        res = mul_mod64(res, i, mod);
    }
    return (seq_t)res;
}
