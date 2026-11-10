/*
 * A339877  a(n) = A336467(A122111(n)).
 * https://oeis.org/A339877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n)=1 for prime n; for composite n, a(n) =
 *            A000265(A105560(n)+1) * a(A064989(n)), where
 *            A105560(n)=prime(bigomega(n)) and A064989 replaces each prime
 *            factor p_i by p_{i-1} (2 -> 1)
 * Range:     u8 n <= 2196, u16 n <= 12166, u32 n <= 12166, u64 n <= 12166,
 *            u128 n <= 12166, big any n
 * Validated: standard, pass, 121 terms through n = 9835 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A339877_isprime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A339877_prevprime(uint64_t p)
{
    if (p <= 2) return 1;
    uint64_t q = p - 1;
    while (!A339877_isprime(q)) q--;
    return q;
}

static uint64_t A339877_nthprime(unsigned k)
{
    uint64_t p = 1;
    unsigned c = 0;
    while (c < k) {
        p++;
        if (A339877_isprime(p)) c++;
    }
    return p;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    while (m > 1) {
        uint64_t x = m;
        uint64_t nm = 1;
        unsigned omega = 0;
        for (uint64_t p = 2; p * p <= x; p += (p == 2 ? 1 : 2)) {
            if (x % p == 0) {
                uint64_t q = A339877_prevprime(p);
                while (x % p == 0) {
                    x /= p;
                    omega++;
                    nm *= q;
                }
            }
        }
        if (x > 1) {
            omega++;
            nm *= A339877_prevprime(x);
        }
        if (omega == 1) break; /* prime */
        uint64_t v = A339877_nthprime(omega) + 1;
        while ((v & 1) == 0) v >>= 1;
        result *= (seq_t)v;
        m = nm;
    }
    return result;
}
