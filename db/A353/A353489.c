/*
 * A353489  If A276086(n) is of the form 4k+3, then a(n) = 1, otherwise a(n) =
 *          0.
 * https://oeis.org/A353489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if A276086(n) == 3 (mod 4), else 0. With n in primorial
 *            base, A276086(n) is the product of prime(k+1)^d_k. It is odd iff
 *            d_0 = 0 (n even), and then it is 3 mod 4 iff the sum of digits
 *            d_k at primes p ≡ 3 (mod 4) is odd
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 122 terms through n = 9735 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    if (m & 1) return (seq_t)0;      /* A276086(n) even */
    m /= 2;
    uint64_t p = 3;
    unsigned parity = 0;
    while (m > 0) {
        uint64_t d = m % p;
        m /= p;
        if (p % 4 == 3) parity ^= (unsigned)(d & 1);
        /* next prime */
        do {
            p += 2;
            uint64_t q = 3;
            int ok = 1;
            for (; q * q <= p; q += 2)
                if (p % q == 0) { ok = 0; break; }
            if (ok) break;
        } while (1);
    }
    return (seq_t)parity;
}
