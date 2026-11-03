/*
 * A351300  a(n) = n^5 * Product_{p|n, p prime} (1 + 1/p^5).
 * https://oeis.org/A351300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p^e‖n} (p^(5*e) + p^(5*e-5))
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 83, u64 n <= 7085,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 49 terms through n = 9257 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to a(n) = n^5 * Product_{p|n}(1 + 1/p^5) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n) {
    if (n <= 1u) return (seq_t)1;
    seq_t result = (seq_t)1;
    uint64_t tmp = (uint64_t)n;
    for (uint64_t p = 2; p * p <= tmp; p += (p == 2 ? 1 : 2)) {
        if (tmp % p == 0) {
            unsigned int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            seq_t p5e = ipow((seq_t)p, (seq_t)(5u * e));
            seq_t p5e1 = ipow((seq_t)p, (seq_t)(5u * e - 5u));
            result *= (p5e + p5e1);
        }
    }
    if (tmp > 1) {
        /* remaining prime factor with exponent 1 */
        seq_t p5e = ipow((seq_t)tmp, (seq_t)5u);
        seq_t p5e1 = (seq_t)1;
        result *= (p5e + p5e1);
    }
    return result;
}
