/*
 * A247815  Number of primes in n-th row of triangle A077581.
 * https://oeis.org/A247815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000720(A069213(n)) - A001221(n), where A069213(n) is the
 *            n-th positive integer coprime to n
 * Range:     u8 n <= 419, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 8863 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static idx_t A247815_gcd(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* v = n-th positive integer coprime to n */
    idx_t v = 0, cnt = 0;
    while (cnt < n) {
        ++v;
        if (A247815_gcd(v, n) == 1) ++cnt;
    }

    /* pi(v) by sieve */
    char *s = (char *)malloc((size_t)v + 1);
    if (!s) return 0;
    memset(s, 1, (size_t)v + 1);
    s[0] = 0;
    if (v >= 1) s[1] = 0;
    for (idx_t i = 2; i * i <= v; i++) {
        if (s[i]) {
            for (idx_t j = i * i; j <= v; j += i) s[j] = 0;
        }
    }
    idx_t pi = 0;
    for (idx_t i = 2; i <= v; i++) if (s[i]) pi++;
    free(s);

    /* omega(n) */
    idx_t om = 0;
    idx_t t = n;
    for (idx_t d = 2; d <= t / d; d++) {
        if (t % d == 0) {
            om++;
            while (t % d == 0) t /= d;
        }
    }
    if (t > 1) om++;

    return (seq_t)(pi - om);
}
