/*
 * A323164  a(n) = A000720(A323075(n)).
 * https://oeis.org/A323164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000720(A323075(n)); A323075(n) is found by iterating x
 *            -> 1+(x-x/spf(x)) until a prime or 1 is reached, then pi is
 *            counted with a sieve
 * Range:     u8 n <= 1618, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8961 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static bool A323164_isprime(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

static uint64_t A323164_spf(uint64_t n)
{
    if (n % 2 == 0) return 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return d;
    return n;
}

static uint64_t A323164_pi(uint64_t m)
{
    if (m < 2) return 0;
    char *s = (char *)malloc((size_t)m + 1);
    if (!s) return 0;
    memset(s, 1, (size_t)m + 1);
    s[0] = 0;
    s[1] = 0;
    for (uint64_t i = 2; i * i <= m; i++)
        if (s[i])
            for (uint64_t j = i * i; j <= m; j += i) s[j] = 0;
    uint64_t c = 0;
    for (uint64_t i = 2; i <= m; i++) c += s[i] ? 1 : 0;
    free(s);
    return c;
}

seq_t a(idx_t n)
{
    uint64_t cur = (uint64_t)n;
    if (cur <= 1) return (seq_t)0;
    while (cur > 1 && !A323164_isprime(cur)) {
        uint64_t p = A323164_spf(cur);
        cur = 1 + (cur - cur / p);
    }
    return (seq_t)A323164_pi(cur);
}
