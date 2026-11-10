/*
 * A351451  a(n) = A064989(A092261(A003961(n))).
 * https://oeis.org/A351451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   multiplicative; a(p) = A064989(nextprime(p)+1), a(p^e) = 1 for e
 *            > 1; A064989 is multiplicative with a(2^e)=1,
 *            a(r^e)=prevprime(r)^e for odd primes r
 * Range:     u8 n <= 522, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 111 terms through n = 9729 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint64_t A351451_prev_prime(uint64_t r)
{
    uint64_t p = r - 1;
    while (!is_prime_u64(p)) --p;
    return p;
}

static uint64_t A351451_next_prime(uint64_t p)
{
    uint64_t q = p + 1;
    while (!is_prime_u64(q)) ++q;
    return q;
}

/* A064989 of m (m >= 1) */
static seq_t A351451_A064989(uint64_t m)
{
    seq_t res = 1;
    while ((m & 1) == 0) m >>= 1;
    for (uint64_t r = 3; r <= m / r; r += 2) {
        if (m % r == 0) {
            uint64_t e = 0;
            while (m % r == 0) { m /= r; e++; }
            seq_t b = (seq_t)A351451_prev_prime(r);
            while (e--) res *= b;
        }
    }
    if (m > 1) res *= (seq_t)A351451_prev_prime(m);
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t m = (uint64_t)n;
    seq_t res = 1;
    uint64_t p;
    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            if (e == 1) {
                uint64_t q = A351451_next_prime(p);
                res *= A351451_A064989(q + 1);
            }
        }
    }
    if (m > 1) {
        uint64_t q = A351451_next_prime(m);
        res *= A351451_A064989(q + 1);
    }
    return res;
}
