/*
 * A354344  a(n) = 1 if n is x * A005383(i), where x is either 2, 3, 8, 9 or
 *          15 and i > 2 [i.e., A005383(i) > 5], otherwise 0.
 * https://oeis.org/A354344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = x*p with x in {2,3,8,9,15}, p prime, p > 5 and
 *            (p+1)/2 prime (p in A005383, p > 5); else 0
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 141 terms through n = 9796 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static bool A005383_big(uint64_t p) {
    if (p <= 5) return false;
    return is_prime_u64(p) && is_prime_u64((p + 1) / 2);
}

seq_t a(idx_t n) {
    static const unsigned xs[5] = {2, 3, 8, 9, 15};
    for (int i = 0; i < 5; i++) {
        if (n % xs[i] == 0 && A005383_big((uint64_t)(n / xs[i])))
            return (seq_t)1;
    }
    return (seq_t)0;
}
