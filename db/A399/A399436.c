/*
 * A399436  a(n) is the denominator of the probability that two heads will
 *          turn up in succession in tossing n times a coin.
 * https://oeis.org/A399436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator(1 - Fib(n+2)/2^n)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the least power of 2 dividing the denominator of the
 * probability that two consecutive heads appear in n tosses.  */
#include "../utils/pow2.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute F_{n+2} modulo 2^64.  The low 64 bits are enough to
     * determine its exponent of 2 for any n ≤ 10 000.  */
    uint64_t f_prev = 0;          /* F_0 */
    uint64_t f_curr = 1;          /* F_1 */
    idx_t k = n + 2;

    if (k == 0) return pow2(0);  /* never happens for n≥1 */
    if (k == 1) f_prev = 1;      /* F_1 */

    for (idx_t i = 2; i <= k; ++i) {
        uint64_t tmp = f_prev + f_curr;   /* overflow wraps modulo 2^64 */
        f_prev = f_curr;
        f_curr = tmp;
    }
    uint64_t fib_mod = f_curr;  /* F_{n+2} mod 2^64 */

    /* Determine the 2‑adic valuation v2(F_{n+2}) from the low bits. */
    unsigned int v = 0;
    if (fib_mod != 0) {
        while ((fib_mod & 1ULL) == 0) {
            fib_mod >>= 1;
            ++v;
        }
    } else {
        /* fib_mod == 0 cannot occur for n ≤ 10 000; handle gracefully. */
        v = 64;
    }

    /* The desired denominator is 2^(n - v). */
    idx_t exponent = n - v;
    if (exponent > n) exponent = n;  /* safety, should never happen */

    return pow2(exponent);
}
