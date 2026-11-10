/*
 * A286477  Ordinal transform of A032742, starting from its first term a(1) =
 *          1.
 * https://oeis.org/A286477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for prime n, a(n) = 1 + pi(n); for composite n, a(n) =
 *            pi(spf(n)), where pi is the prime-counting function and spf is
 *            the smallest prime factor (ordinal transform of A032742)
 * Range:     u8 n <= 1612, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 129 terms through n = 9570 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A286477_count_primes(uint64_t limit)
{
    if (limit < 2)
        return 0;
    unsigned char *s = (unsigned char *)malloc((size_t)limit + 1);
    if (!s)
        return 0;
    for (uint64_t i = 0; i <= limit; ++i)
        s[i] = 1;
    s[0] = 0;
    s[1] = 0;
    uint64_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (s[i]) {
            ++cnt;
            if (i <= limit / i)
                for (uint64_t j = i * i; j <= limit; j += i)
                    s[j] = 0;
        }
    }
    free(s);
    return cnt;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    if (nn <= 1)
        return (seq_t)1;

    uint64_t spf = nn;
    if ((nn % 2) == 0)
        spf = 2;
    else {
        for (uint64_t d = 3; d <= nn / d; d += 2) {
            if ((nn % d) == 0) {
                spf = d;
                break;
            }
        }
    }

    if (spf == nn)
        return (seq_t)(A286477_count_primes(nn) + 1);
    return (seq_t)A286477_count_primes(spf);
}
