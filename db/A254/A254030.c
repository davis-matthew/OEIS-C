/*
 * A254030  a(n) = 1*4^n + 2*3^n + 3*2^n + 4*1^n.
 * https://oeis.org/A254030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1*4^n + 2*3^n + 3*2^n + 4*1^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 982 (2026-11-01)
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n) {
    seq_t v4 = pow4(n);                  /* 4^n */
    seq_t v3 = pow3(n);                  /* 3^n */
    seq_t v2 = pow2(n);                  /* 2^n */

    seq_t result = v4;                   /* start with 1*4^n */
    result += mul_small(v3, 2);          /* add 2*3^n */
    result += mul_small(v2, 3);          /* add 3*2^n */
    result += (seq_t)4;                  /* add 4*1^n (always 4) */

    return result;
}
