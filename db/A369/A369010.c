/*
 * A369010  Exponential of Mangoldt function M(n) applied to primorial base
 *          exp-function: a(n) = A014963(A276086(n)).
 * https://oeis.org/A369010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014963(A276086(n)); A276086(n) = prod prime(k+1)^d_k
 *            with d_k the primorial-base digits of n, so a(n) = p if exactly
 *            one digit d_k is nonzero (p = prime(k+1)), else 1
 * Range:     u8 n <= 30030, u16 n <= 30030, u32 n <= 30030, u64 n <= 30030,
 *            u128 n <= 30030, big any n
 * Validated: standard, pass, 122 terms through n = 9278 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A369010_next_prime(uint64_t p)
{
    uint64_t c = p + 1;
    for (;; ++c) {
        int ok = 1;
        for (uint64_t d = 2; d * d <= c; ++d) {
            if (c % d == 0) { ok = 0; break; }
        }
        if (ok) return c;
    }
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    uint64_t p = 2;
    int count = 0;
    uint64_t last = 1;
    while (m > 0) {
        uint64_t d = m % p;
        m /= p;
        if (d != 0) {
            ++count;
            last = p;
        }
        p = A369010_next_prime(p);
    }
    if (count == 1)
        return (seq_t)last;
    return (seq_t)1;
}
