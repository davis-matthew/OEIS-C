/*
 * A327937  Multiplicative with a(p^e) = p^(p-1) if e >= p, otherwise a(p^e) =
 *          p^e.
 * https://oeis.org/A327937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is multiplicative with a(p^e) = p^(p-1) if e >= p,
 *            otherwise a(p^e) = p^e
 * Range:     u8 n <= 256, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 104 terms through n = 7532 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n) {
    /* OEIS offset starts at 1; n >= 1 */
    if (n == 0) return 0;

    seq_t result = (seq_t)1;
    uint64_t t = (uint64_t)n;

    /* Handle factor 2 separately for speed */
    if ((t & 1ULL) == 0) {
        int e = 0;
        while ((t & 1ULL) == 0) {
            t >>= 1;
            e++;
        }
        seq_t factor = (e >= 2) ? ipow((seq_t)2, (seq_t)(2 - 1))
                                : ipow((seq_t)2, (seq_t)e);
        result = seq_mul(result, factor);
    }

    /* Trial division by odd primes */
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) {
                t /= p;
                e++;
            }
            seq_t factor = ((uint64_t)e >= p) ? ipow((seq_t)p, (seq_t)(p - 1))
                                              : ipow((seq_t)p, (seq_t)e);
            result = seq_mul(result, factor);
        }
    }

    /* If a prime factor larger than sqrt(t) remains */
    if (t > 1) {
        uint64_t p = t;                 /* exponent e = 1 */
        seq_t factor = ipow((seq_t)p, (seq_t)1);
        result = seq_mul(result, factor);
    }

    return result;
}
