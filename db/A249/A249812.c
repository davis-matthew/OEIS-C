/*
 * A249812  Permutation of natural numbers: a(n) = A000079(A055396(n+1)-1) *
 *          ((2*A078898(n+1))-1).
 * https://oeis.org/A249812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000079(A055396(n+1)-1) * ((2*A078898(n+1))-1), where p =
 *            smallest prime factor of n+1, A055396 = pi(p), A078898 =
 *            #{k<=n+1 : spf(k)=p}
 * Range:     u8 n <= 21, u16 n <= 57, u32 n <= 135, u64 n <= 311,
 *            u128 n <= 725, big any n
 * Validated: standard, pass, 100 terms through n = 803 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = n + 1;
    idx_t p = m;
    for (idx_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) { p = d; break; }
    }

    /* sieve primes up to p */
    unsigned char *comp = (unsigned char *)malloc((size_t)p + 1);
    if (!comp) return (seq_t)0;
    for (idx_t i = 0; i <= p; ++i) comp[i] = 0;
    idx_t pi = 0;
    for (idx_t i = 2; i <= p; ++i) {
        if (!comp[i]) {
            ++pi;
            for (idx_t j = i * i; j <= p && j >= i; j += i) comp[j] = 1;
        }
    }

    /* count j in 1..m/p with no prime factor < p */
    idx_t L = m / p;
    unsigned char *mark = (unsigned char *)malloc((size_t)L + 1);
    if (!mark) { free(comp); return (seq_t)0; }
    for (idx_t i = 0; i <= L; ++i) mark[i] = 0;
    for (idx_t q = 2; q < p; ++q) {
        if (comp[q]) continue;
        for (idx_t j = q; j <= L; j += q) mark[j] = 1;
    }
    idx_t cnt = 0;
    for (idx_t j = 1; j <= L; ++j)
        if (!mark[j]) ++cnt;

    free(mark);
    free(comp);

    seq_t result = (seq_t)(2 * cnt - 1);
    for (idx_t i = 1; i < pi; ++i) result *= 2;
    return result;
}
