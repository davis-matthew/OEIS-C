/*
 * A286253  Compound filter: a(n) = P(A055396(n), A001511(1+n)), where P(n,k)
 *          is sequence A000027 used as a pairing function.
 * https://oeis.org/A286253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2 + (x+k)^2 - x - 3k)/2 with x = A055396(n), k =
 *            A001511(n+1)
 * Range:     u8 n <= 70, u16 n <= 2398, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9756 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A055396_index(uint64_t n)
{
    if (n <= 1) return 0;
    uint64_t spf = n;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) { spf = p; break; }
    }
    /* count primes <= spf with a sieve */
    unsigned char *s = (unsigned char *)malloc((size_t)spf + 1);
    if (!s) return 0;
    for (uint64_t i = 0; i <= spf; ++i) s[i] = 1;
    uint64_t cnt = 0;
    for (uint64_t i = 2; i <= spf; ++i) {
        if (s[i]) {
            ++cnt;
            for (uint64_t j = i * i; j <= spf && j >= i; j += i) s[j] = 0;
        }
    }
    free(s);
    return cnt;
}

static unsigned A001511_val(uint64_t n)
{
    unsigned r = 1;
    while ((n & 1) == 0) { ++r; n >>= 1; }
    return r;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)A055396_index((uint64_t)n);
    seq_t k = (seq_t)A001511_val((uint64_t)n + 1);
    seq_t s = x + k;
    return (s * s + 2 - x - 3 * k) / 2;
}
