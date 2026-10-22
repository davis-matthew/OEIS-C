/*
 * A342002  Čiurlionis sequence: Arithmetic derivative without its inherited
 *          divisor applied to the primorial base exp-function: a(n) =
 *          A342001(A276086(n)).
 * https://oeis.org/A342002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = rad( A276086(n) ) * Σ_{p^e||A276086(n)} e / p where
 *            A276086(n) is obtained by writing n in mixed radix with bases
 *            successive primes and then interpreting the digits as exponents
 *            of the same primes. rad(m) = product of the distinct primes that
 *            divide m. The function returns a(n) for any n ≥ 0
 * Range:     u8 n <= 40, u16 n <= 2578, u32 n <= 8661, u64 n <= 8661,
 *            u128 n <= 8661, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const unsigned primes[] = {
    2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u, 23u, 29u,
    31u, 37u, 41u, 43u, 47u, 53u, 59u, 61u, 67u, 71u,
    73u, 79u, 83u, 89u, 97u, 101u, 103u, 107u, 109u, 113u
};
#define PRIME_COUNT (sizeof(primes)/sizeof(primes[0]))

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* a(0) = 0 */

    idx_t tmp = n;                         /* number to decompose */
    seq_t rad = 1;                         /* product of distinct primes with non‑zero digit */
    unsigned digits[64] = {0};             /* small array for the digits */
    size_t idx = 0;

    /* decompose n using primes as bases, store the digits (exponents) */
    while (tmp > 0 || idx == 0) {
        if (idx >= PRIME_COUNT) break;     /* safety: should not happen for n ≤ 10000 */
        unsigned p = primes[idx];
        unsigned d = (unsigned)(tmp % p);
        digits[idx] = d;
        tmp = tmp / p;
        if (d > 0) rad *= (seq_t)p;        /* include prime in rad if exponent > 0 */
        ++idx;
    }

    /* compute the result: rad * Σ d_i / p_i  =  Σ (rad/p_i) * d_i */
    seq_t result = 0;
    for (size_t j = 0; j < idx; ++j) {
        unsigned d = digits[j];
        if (d == 0) continue;
        seq_t term = (rad / (seq_t)primes[j]) * (seq_t)d;
        result += term;
    }
    return result;
}
