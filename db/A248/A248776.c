/*
 * A248776  Greatest 8th power integer that divides n!
 * https://oeis.org/A248776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = \prod_{p≤n} p^{8*\lfloor v_p(n!)/8\rfloor}
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 27, u64 n <= 35, u128 n <= 59,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 882 (2026-11-01)
 * Helpers:   build_primes, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_primes.h"
#include "../utils/ipow.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)1;          /* offset is 1 */

    /* cached prime list up to the maximum n requested so far */
    static idx_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t max_n = 0;

    /* rebuild prime table if we need larger primes */
    if (n > max_n) {
        if (primes) free(primes);
        build_primes(n, &primes, &prime_cnt);
        max_n = n;
    }

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_cnt && primes[i] <= n; ++i) {
        idx_t p      = primes[i];
        idx_t vp     = 0;            /* exponent of p in n! */
        idx_t power  = p;
        while (power <= n) {
            vp += n / power;
            if (power > n / p) break;    /* avoid overflow of power*p */
            power *= p;
        }
        idx_t e8 = vp / 8;            /* how many 8th‑powers of p to include */
        if (e8 == 0) continue;
        seq_t p_pow = ipow((seq_t)p, (seq_t)(8 * e8));
        result = seq_mul(result, p_pow);
    }

    return result;
}
