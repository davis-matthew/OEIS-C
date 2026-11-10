/*
 * A364540  Numbers k for which A005940(k) < k.
 * https://oeis.org/A364540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A005940(k) < k (search over k, computing the
 *            Doudna value A005940(k) with early exit once it reaches k)
 * Range:     u8 n <= 14, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A364540_less(idx_t k) {
    static const unsigned primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61,
        67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137,
        139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211,
        223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283,
        293, 307, 311, 313, 317
    };
    idx_t m = k - 1;
    seq_t kk = (seq_t)k;
    seq_t result = 1;
    unsigned i = 1;
    while (m > 0) {
        if (m & 1) {
            if (!(result < kk)) return false;
            result *= (seq_t)primes[i - 1];
            if (!(result < kk)) return false;
            m = (m - 1) >> 1;
        } else {
            m >>= 1;
            i++;
        }
    }
    return result < kk;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t k = 1;
    for (;; k++) {
        if (A364540_less(k)) {
            count++;
            if (count == n) return (seq_t)k;
        }
    }
}
