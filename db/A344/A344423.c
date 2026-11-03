/*
 * A344423  a(n) = 10^(2*n+2) + 111*10^n + 1.
 * https://oeis.org/A344423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^(2*n+2) + 111*10^n + 1
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 440 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
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

seq_t a(idx_t n)
{
    /* Compute 10^n and 10^(2*n+2) (the latter directly) */
    seq_t ten_pow_n = pow10(n);
    seq_t ten_pow_2n2 = pow10(2 * n + 2);

    /* a(n) = 10^(2*n+2) + 111*10^n + 1 */
    return ten_pow_2n2 + mul_small(ten_pow_n, 111) + 1;
}
