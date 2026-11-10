/*
 * A391864  Numbers k such that A003415(k) == A276085(k) (mod 2310), where
 *          A003415 is the arithmetic derivative and A276085 is the primorial
 *          base log-function.
 * https://oeis.org/A391864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A003415(k) == A276085(k) (mod 2310);
 *            found by an spf sieve and scan, cached
 * Range:     u8 n <= 6, u16 n <= 57, u32 n <= 23520, u64 n <= 23520,
 *            u128 n <= 23520, big any n
 * Validated: standard, pass, 65 terms through n = 9352 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *terms_cache = NULL;
static size_t terms_cnt = 0;
static uint64_t scan_limit = 0;

static void rescan_to(uint64_t lim) {
    uint32_t *spf = (uint32_t *)calloc((size_t)lim + 1, sizeof(uint32_t));
    uint64_t i, j;
    for (i = 2; i <= lim; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= lim; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    free(terms_cache);
    terms_cache = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)lim / 200 + 1024));
    size_t cap = (size_t)lim / 200 + 1024;
    terms_cnt = 0;
    for (uint64_t k = 1; k <= lim; k++) {
        uint64_t d = 0, r = 0, m = k;
        while (m > 1) {
            uint64_t p = spf[m];
            uint64_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            d = (d + (k / p) % 2310 * e) % 2310;
            switch (p) {
                case 2: r += e; break;
                case 3: r += 2 * e; break;
                case 5: r += 6 * e; break;
                case 7: r += 30 * e; break;
                case 11: r += 210 * e; break;
                default: break;
            }
            r %= 2310;
        }
        if (d == r) {
            if (terms_cnt >= cap) {
                cap *= 2;
                terms_cache = (uint64_t *)realloc(terms_cache, sizeof(uint64_t) * cap);
            }
            terms_cache[terms_cnt++] = k;
        }
    }
    free(spf);
    scan_limit = lim;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    while (terms_cnt < (size_t)n) {
        uint64_t lim = scan_limit ? scan_limit * 2 : (1u << 16);
        rescan_to(lim);
    }
    return (seq_t)terms_cache[n - 1];
}
