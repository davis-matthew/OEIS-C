/*
 * A271553  a(n) = H_{n-1}(n,n).
 * https://oeis.org/A271553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, bref
 * Formula:   a(n) = n^(n-1) for n >= 3; a(0)=1; a(1)=2; a(2)=4
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 4 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static seq_t seq_pow_int(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) {
            result = seq_mul(result, base);
        }
        exp >>= 1;
        if (exp) {
            base = seq_mul(base, base);
        } else {
            break;
        }
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 4;
    seq_t base = (seq_t)n;
    return seq_pow_int(base, n - 1);
}
