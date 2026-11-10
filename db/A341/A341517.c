/*
 * A341517  a(n) = mu(A327859(n)), where mu is the Möbius function, A008683.
 * https://oeis.org/A341517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A008683(A276086(A003415(n))): mu is nonzero only when
 *            every primorial-base digit of n' is 0 or 1, and then equals
 *            (-1)^(number of 1 digits)
 * Range:     i8 n <= 65537, i16 n <= 65537, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 122 terms through n = 9494 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Arithmetic derivative n' computed in a wide native type, so it cannot overflow narrow seq_t builds */
static unsigned __int128 A341517_deriv(idx_t n)
{
    if (n < 2) return 0;
    unsigned __int128 result = 0;
    uint64_t rem = (uint64_t)n;
    for (uint64_t p = 2; p <= rem / p; ++p) {
        if (rem % p == 0) {
            unsigned e = 0;
            while (rem % p == 0) { rem /= p; ++e; }
            result += (unsigned __int128)((uint64_t)n / p) * e;
        }
    }
    if (rem > 1)
        result += (unsigned __int128)((uint64_t)n / rem);
    return result;
}

static bool A341517_is_prime_small(uint32_t c)
{
    if (c < 2) return false;
    for (uint32_t q = 2; q * q <= c; ++q)
        if (c % q == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    unsigned __int128 m = A341517_deriv(n);
    uint32_t p = 2;
    int cnt = 0;
    while (m != 0) {
        unsigned __int128 digit = m % p;
        if (digit > 1) return (seq_t)0;
        if (digit == 1) ++cnt;
        m /= p;
        uint32_t c = p + 1;
        while (!A341517_is_prime_small(c)) ++c;
        p = c;
    }
    return (cnt & 1) ? (seq_t)(-1) : (seq_t)1;
}
