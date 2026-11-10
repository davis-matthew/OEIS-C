/*
 * A353488  If A276086(n) is of the form 4k+1, then a(n) = 1, otherwise a(n) =
 *          0.
 * https://oeis.org/A353488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if A276086(n) == 1 (mod 4), else 0. A276086(n) =
 *            Product_k prime(k+1)^d_k with d_k the primorial-base digits of
 *            n. It is 1 mod 4 iff d_0 = 0 and the sum of d_k over primes p ==
 *            3 (mod 4) is even
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 122 terms through n = 9884 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A353488_next_prime(uint64_t p)
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
    uint64_t m = (uint64_t)n;
    uint64_t p = 2;
    uint64_t cnt = 0;
    while (m > 0) {
        uint64_t d = m % p;
        m /= p;
        if (p == 2) {
            if (d == 1) return (seq_t)0;
        } else if ((p & 3) == 3) {
            cnt += d;
        }
        p = A353488_next_prime(p);
    }
    return (seq_t)((cnt & 1) ? 0 : 1);
}
