/*
 * A373138  Numbers k such that A276085(k) is a multiple of 8, where A276085
 *          is the primorial base log-function.
 * https://oeis.org/A373138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A276085(k) == 0 (mod 8), A276085(k) =
 *            Sum_{p^e || k} e*(product of primes < p); computed by a sieve
 *            with arithmetic mod 8
 * Range:     u8 n <= 37, u16 n <= 10898, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 83 terms through n = 9900 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *terms_cache = NULL;
static uint64_t terms_count = 0;

static void build_terms(uint64_t limit)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    uint8_t *g = (uint8_t *)calloc((size_t)limit + 1, 1);
    uint8_t *fp = (uint8_t *)calloc((size_t)limit + 1, 1);
    uint32_t *out = (uint32_t *)malloc(((size_t)limit + 1) * sizeof(uint32_t));
    uint64_t cnt = 0;
    unsigned run = 1; /* product of primes below current, mod 8 */
    uint64_t k, j;
    for (k = 1; k <= limit; ++k) {
        if (k >= 2) {
            if (spf[k] == 0) {
                spf[k] = (uint32_t)k;
                fp[k] = (uint8_t)run;
                run = (run * (unsigned)(k & 7)) & 7;
                for (j = k * k; j <= limit; j += k)
                    if (spf[j] == 0) spf[j] = (uint32_t)k;
            }
            uint32_t p = spf[k];
            g[k] = (uint8_t)((g[k / p] + fp[p]) & 7);
        }
        if (g[k] == 0) out[cnt++] = (uint32_t)k;
    }
    free(spf);
    free(g);
    free(fp);
    free(terms_cache);
    terms_cache = out;
    terms_count = cnt;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint64_t limit = 4096;
    while (terms_count < (uint64_t)n) {
        if (terms_cache != NULL) {
            limit = terms_count > 0 ? (uint64_t)terms_cache[terms_count - 1] * 2 : limit * 2;
            if (limit < 4096) limit = 4096;
            /* ensure growth even when little found */
        }
        build_terms(limit);
        if (terms_count < (uint64_t)n) {
            /* proceed to loop again with doubled limit */
            limit *= 2;
            if (terms_count > 0) {
                uint64_t est = (uint64_t)n * (limit / terms_count + 1);
                if (est > limit) limit = est;
            }
            build_terms(limit);
        }
    }
    return (seq_t)terms_cache[n - 1];
}
