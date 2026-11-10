/*
 * A328389  Maximal digit value in primorial base expansion of A276086(n):
 *          a(n) = A328114(A276086(n)).
 * https://oeis.org/A328389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A328114(A276086(n)): max digit of primorial-base
 *            expansion of prod prime(k+1)^d_k(n), d_k = primorial-base digits
 *            of n
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 1213, u64 n <= 32589,
 *            u128 n <= 32589, big any n
 * Validated: standard, pass, 122 terms through n = 8614 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A328389_next_prime(uint64_t p)
{
    uint64_t c = p + 1;
    if (c <= 2) return 2;
    if ((c & 1) == 0) c++;
    for (;; c += 2) {
        bool ok = true;
        for (uint64_t d = 3; d * d <= c; d += 2) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok) return c;
    }
}

seq_t a(idx_t n)
{
    /* Compute m = A276086(n) */
    seq_t m = 1;
    uint64_t rem = (uint64_t)n;
    uint64_t p = 2;
    while (rem > 0) {
        uint64_t d = rem % p;
        rem /= p;
        for (uint64_t i = 0; i < d; i++)
            m = m * (seq_t)p;
        p = A328389_next_prime(p);
    }

    /* Max digit of m in primorial base (mixed radix 2,3,5,7,...) */
    seq_t maxd = 0;
    p = 2;
    while (m > 0) {
        seq_t d = m % (seq_t)p;
        m = m / (seq_t)p;
        if (d > maxd) maxd = d;
        p = A328389_next_prime(p);
    }
    return maxd;
}
