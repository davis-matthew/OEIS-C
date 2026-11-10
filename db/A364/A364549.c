/*
 * A364549  Odd numbers k that divide A005941(k).
 * https://oeis.org/A364549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th odd k such that k divides A005941(k) = 1 +
 *            A156552(k); found by a sieve/search, evaluating A156552(k) mod k
 *            with Montgomery arithmetic
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A364549_found = NULL;
static size_t A364549_cnt = 0, A364549_cap = 0;
static uint64_t A364549_scanned = 0;   /* all odd k <= scanned have been tested */
static uint64_t A364549_limit = 0;

static void A364549_push(uint64_t k) {
    if (A364549_cnt == A364549_cap) {
        A364549_cap = A364549_cap ? A364549_cap * 2 : 64;
        A364549_found = (uint64_t *)realloc(A364549_found, A364549_cap * sizeof(uint64_t));
    }
    A364549_found[A364549_cnt++] = k;
}

#define A364549_FLAG 0x80000000u

static void A364549_scan(uint64_t L) {
    uint32_t *spf = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    uint32_t *pr = (uint32_t *)malloc((L / 8 + 100) * sizeof(uint32_t));
    size_t np = 0;
    for (uint64_t i = 2; i <= L; i++) {
        uint32_t sv;
        if (spf[i] == 0) {
            pr[np++] = (uint32_t)i;
            spf[i] = A364549_FLAG | (uint32_t)np;
            sv = (uint32_t)i;
        } else sv = spf[i];
        for (size_t t = 0; t < np; t++) {
            uint32_t p = pr[t];
            if (p > sv || (uint64_t)i * p > L) break;
            spf[i * p] = p;
        }
    }
    uint64_t start = A364549_scanned + 1;
    if (start % 2 == 0) start++;
    for (uint64_t k = start; k <= L; k += 2) {
        if (k == 1) { A364549_push(1); continue; }
        uint32_t n = (uint32_t)k;
        /* Montgomery setup */
        uint32_t inv = n;
        for (int i = 0; i < 5; i++) inv *= 2u - n * inv;
        uint32_t ninv = 0u - inv;
        uint32_t one = (uint32_t)((((uint64_t)1) << 32) % n);
        uint32_t sum = 0;
        uint64_t j = 0;
        uint64_t m = k;
        while (m > 1) {
            uint32_t p, idx;
            if (spf[m] & A364549_FLAG) { p = (uint32_t)m; idx = spf[m] & 0x7fffffffu; }
            else { p = spf[m]; idx = spf[p] & 0x7fffffffu; }
            uint64_t e = (uint64_t)idx - 1 + j;
            uint32_t x = one;
            if (e > 0) {
                int top = 63;
                while (!((e >> top) & 1)) top--;
                for (int b = top; b >= 0; b--) {
                    uint64_t T = (uint64_t)x * x;
                    uint32_t mm = (uint32_t)T * ninv;
                    uint64_t t = (T + (uint64_t)mm * n) >> 32;
                    x = (uint32_t)(t >= n ? t - n : t);
                    if ((e >> b) & 1) {
                        uint64_t d = (uint64_t)x * 2;
                        x = (uint32_t)(d >= n ? d - n : d);
                    }
                }
            }
            for (;;) {
                sum += x; if (sum >= n || sum < x) sum -= n;
                j++;
                m /= p;
                if (m % p != 0) break;
                uint64_t d = (uint64_t)x * 2;
                x = (uint32_t)(d >= n ? d - n : d);
            }
        }
        uint64_t tot = (uint64_t)sum + one;
        if (tot >= n) tot -= n;
        if (tot == 0) A364549_push(k);
    }
    A364549_scanned = L;
    free(spf);
    free(pr);
}

seq_t a(idx_t n) {
    while (A364549_cnt < n) {
        A364549_limit = A364549_limit ? A364549_limit * 2 : (1u << 16);
        A364549_scan(A364549_limit);
    }
    return (seq_t)A364549_found[n - 1];
}
