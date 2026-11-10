/*
 * A268674  a(1) = 1, after which, for odd numbers: a(n) = A078898(n)-th
 *          number k for which A055396(k) = A055396(n)-1, and for even
 *          numbers: a(n) = a(A000265(n)).
 * https://oeis.org/A268674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n)=a(odd part of n) for even n; for odd n>1 with least
 *            prime p=prime(i), n=p*t: a(n) = c-th number k with
 *            A055396(k)=i-1, where c = A078898(n) = #{t'<=t : t' has no prime
 *            factor < p}; computed directly by counting/search
 * Range:     u8 n <= 262, u16 n <= 32769, u32 n <= 32769, u64 n <= 32769,
 *            u128 n <= 32769, big any n
 * Validated: standard, pass, 104 terms through n = 9941 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool a268674_ok(idx_t x, idx_t bound)
{
    /* true if x has no prime factor < bound */
    for (idx_t d = 2; d < bound && d <= x; ++d)
        if (x % d == 0) return false;
    return true;
}

static bool a268674_isprime(idx_t x)
{
    if (x < 2) return false;
    for (idx_t d = 2; d * d <= x; ++d)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    while (n > 1 && (n & 1) == 0) n >>= 1;
    if (n <= 1) return (seq_t)1;

    idx_t p = n;
    for (idx_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) { p = d; break; }
    idx_t t = n / p;

    idx_t c = 0;
    for (idx_t u = 1; u <= t; ++u)
        if (a268674_ok(u, p)) ++c;

    idx_t q = p - 1;
    while (!a268674_isprime(q)) --q;

    idx_t cnt = 0, s = 0;
    while (cnt < c) {
        ++s;
        if (a268674_ok(s, q)) ++cnt;
    }
    return (seq_t)q * (seq_t)s;
}
