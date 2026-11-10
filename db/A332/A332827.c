/*
 * A332827  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => f(i) = f(j), where f(n) = A003557(A332824(n)) for all other
 *          numbers, except f(1) = 0.
 * https://oeis.org/A332827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of first occurrence of f(n) among f(1..n), with
 *            f(n) = A003557(A332824(n)), f(1) = 0; computed by sieve:
 *            A332824(n) has exponent of p_k = #{d|n : bit k of phi(d) set},
 *            A003557 lowers each exponent by 1 (min 0); signatures are hashed
 * Range:     u8 n <= 403, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9561 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

static uint64_t *S332827_res = NULL;
static uint64_t S332827_lim = 0;

static void S332827_build(uint64_t limit)
{
    uint64_t i, j, m;
    uint32_t *phi = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    uint64_t tsize = 4;
    while (tsize < 2 * limit + 2) tsize <<= 1;
    uint64_t *k1 = (uint64_t *)calloc(tsize, sizeof(uint64_t));
    uint64_t *k2 = (uint64_t *)calloc(tsize, sizeof(uint64_t));
    uint64_t *val = (uint64_t *)calloc(tsize, sizeof(uint64_t));
    uint64_t *res = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));

    for (i = 0; i <= limit; i++) phi[i] = (uint32_t)i;
    for (i = 2; i <= limit; i++)
        if (phi[i] == i)
            for (j = i; j <= limit; j += i) phi[j] -= phi[j] / (uint32_t)i;

    uint64_t next = 1;
    res[0] = 0;
    for (m = 1; m <= limit; m++) {
        if (m == 1) { res[1] = next++; continue; }
        int c[64];
        memset(c, 0, sizeof(c));
        for (uint64_t d = 1; d * d <= m; d++) {
            if (m % d) continue;
            uint32_t p = phi[d];
            for (int k = 0; p; k++, p >>= 1) if (p & 1U) c[k]++;
            uint64_t o = m / d;
            if (o != d) {
                p = phi[o];
                for (int k = 0; p; k++, p >>= 1) if (p & 1U) c[k]++;
            }
        }
        uint64_t h1 = 1469598103934665603ULL, h2 = 0x9E3779B97F4A7C15ULL;
        for (int k = 0; k < 64; k++) {
            uint64_t e = c[k] > 1 ? (uint64_t)(c[k] - 1) : 0;
            h1 = (h1 ^ e) * 1099511628211ULL;
            h2 = (h2 + e + 0x7F4A7C15ULL) * 0xBF58476D1CE4E5B9ULL;
            h2 ^= h2 >> 29;
        }
        h1 |= 1; /* nonzero marker for occupied slot */
        uint64_t pos = (h2 ^ (h1 >> 7)) & (tsize - 1);
        for (;;) {
            if (k1[pos] == 0) {
                k1[pos] = h1; k2[pos] = h2; val[pos] = next;
                res[m] = next++;
                break;
            }
            if (k1[pos] == h1 && k2[pos] == h2) { res[m] = val[pos]; break; }
            pos = (pos + 1) & (tsize - 1);
        }
    }
    free(phi); free(k1); free(k2); free(val);
    free(S332827_res);
    S332827_res = res;
    S332827_lim = limit;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    if (nn < 1) return (seq_t)0;
    if (nn > S332827_lim) {
        uint64_t lim = nn;
        if (lim < 2 * S332827_lim && 2 * S332827_lim <= 4000000ULL) lim = 2 * S332827_lim;
        if (lim < 1024 && nn <= 1024) lim = 1024;
        S332827_build(lim);
    }
    return (seq_t)S332827_res[nn];
}
