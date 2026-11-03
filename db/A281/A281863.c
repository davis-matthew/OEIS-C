/*
 * A281863  Alternating powers of 60 and 10 times powers of 60.
 * https://oeis.org/A281863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n) = 60^n, a(2*n+1) = 10*60^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 963 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow.h"

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

seq_t a(idx_t n)
{
    idx_t r = n / 2;
    seq_t base = (seq_t)60;
    seq_t val = ipow(base, (seq_t)r);
    if (n % 2 == 1)
        val = mul_small(val, 10);
    return val;
}
