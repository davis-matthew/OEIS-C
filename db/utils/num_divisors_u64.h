/* num_divisors_u64: the number of divisors of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIVISORS_U64_H
#define OEISC_UTILS_NUM_DIVISORS_U64_H
#include <stdint.h>

static inline seq_t num_divisors_u64(uint64_t x) {
    /* divisor count by trial factorisation */
    if (x <= 1) return 1;
    seq_t res = 1;
    uint64_t p = 2;
    while (p * p <= x) {
        if (x % p == 0) {
            uint64_t exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            res *= (exp + 1);
        }
        p = (p == 2) ? 3 : p + 2; /* skip even numbers after 2 */
    }
    if (x > 1) res *= 2;             /* remaining prime factor */
    return res;
}

#endif
