/*
 * A324654  a(n) = A033879(A276086(n)).
 * https://oeis.org/A324654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A033879(A276086(n)) = 2*m - sigma(m), where m =
 *            A276086(n) = prod prime(k+1)^d_k (d_k = primorial-base digits of
 *            n); sigma is multiplicative so it is built from the same digits
 * Range:     i8 n <= 16, i16 n <= 58, i32 n <= 388, i64 n <= 2302,
 *            i128 n <= 2310, big any n
 * Validated: standard, pass, 85 terms through n = 2225 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A324654_next_prime(uint64_t p)
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
    seq_t m = 1;
    seq_t s = 1;
    uint64_t p = 1;
    uint64_t rem = (uint64_t)n;
    while (rem > 0) {
        p = A324654_next_prime(p);
        uint64_t d = rem % p;
        rem /= p;
        if (d) {
            seq_t term = 1, sum = 1;
            for (uint64_t i = 0; i < d; ++i) {
                term = term * (seq_t)p;
                sum = sum + term;
            }
            m = m * term;
            s = s * sum;
        }
    }
    return (seq_t)2 * m - s;
}
