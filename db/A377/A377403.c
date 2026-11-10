/*
 * A377403  For n >= 2, a(n) is the number of iterations needed for the map: x
 *          -> x / A085392(x) if A085392(x) > 1, otherwise x -> x +
 *          A151800(x), to (the first occurrence of) 2.
 * https://oeis.org/A377403
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bigomega(n) - 1 for n even; a(n) = bigomega(n) - 1 +
 *            bigomega(A013634(A020639(n))) for n odd, where A020639 is the
 *            least prime factor and A013634(m) = nextprime(m) + m
 * Range:     u8 n <= 121, u16 n <= 121, u32 n <= 121, u64 n <= 121,
 *            u128 n <= 121, big any n
 * Validated: partial, pass, 120 terms through n = 121 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static unsigned omega_count_u64(uint64_t m)
{
    unsigned c = 0;
    while ((m & 1ULL) == 0 && m > 0) { c++; m >>= 1; }
    for (uint64_t f = 3; f <= m / f; f += 2) {
        while (m % f == 0) { c++; m /= f; }
    }
    if (m > 1) c++;
    return c;
}

static uint64_t least_prime_factor_u64(uint64_t m)
{
    if ((m & 1ULL) == 0) return 2;
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) return f;
    }
    return m;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    unsigned big = omega_count_u64(m);
    seq_t res = (seq_t)big - 1;
    if ((m & 1ULL) == 0) return res;

    uint64_t p = least_prime_factor_u64(m);
    uint64_t q = p + 1;
    if (q <= 2) q = 2;
    else if ((q & 1ULL) == 0) q++;
    while (!is_prime_u64(q)) q += 2;
    uint64_t s = p + q;
    res += (seq_t)omega_count_u64(s);
    return res;
}
