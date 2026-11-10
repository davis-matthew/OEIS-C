/*
 * A275464  a(n) = n - A038802(n).
 * https://oeis.org/A275464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A038802(n), where A038802(n) = number of primes less
 *            than the least prime factor of 2n+1 (computed by trial division
 *            and a sieve)
 * Range:     u8 n <= 258, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 98 terms through n = 995 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint8_t *c275464_comp = NULL;
static uint64_t c275464_lim = 0;

static void c275464_sieve(uint64_t limit)
{
    if (limit <= c275464_lim && c275464_comp) return;
    if (limit < 2 * c275464_lim) limit = 2 * c275464_lim;
    if (limit < 100) limit = 100;
    uint8_t *f = (uint8_t *)calloc((size_t)(limit + 1), 1);
    if (!f) return;
    for (uint64_t i = 2; i * i <= limit; i++)
        if (!f[i])
            for (uint64_t j = i * i; j <= limit; j += i) f[j] = 1;
    free(c275464_comp);
    c275464_comp = f;
    c275464_lim = limit;
}

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n * 2 + 1;
    uint64_t spf = x;
    for (uint64_t d = 3; d <= x / d; d += 2) {
        if (x % d == 0) { spf = d; break; }
    }
    c275464_sieve(spf);
    uint64_t cnt = 0;
    for (uint64_t p = 2; p < spf; p++)
        if (!c275464_comp[p]) cnt++;
    return (seq_t)n - (seq_t)cnt;
}
