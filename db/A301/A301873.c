/*
 * A301873  Expansion of Product_{k>=1} 1/(1 - x^k)^A007437(k).
 * https://oeis.org/A301873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Product_{k>=1} 1/(1 - x^k)^A007437(k), A007437(k) =
 *            Sum_{d|k} d*(d+1)/2; computed by multiplying in each factor
 *            sum_{j>=0} binomial(e+j-1, j) x^(k*j)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 29, u64 n <= 69, u128 n <= 169,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 998 (2026-11-08)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"
#include "../utils/gcd.h"

static seq_t *A301873_tab = 0;
static idx_t A301873_cnt = 0;

static void A301873_build(idx_t m) {
    seq_t *e = mem_new(seq_t, m + 1);
    seq_t *t = mem_new(seq_t, m + 1);
    seq_t *cb = mem_new(seq_t, m + 1);
    for (idx_t d = 1; d <= m; d++) {
        seq_t sd = (seq_t)d;
        seq_t tri = sd * (sd + 1) / 2;
        for (idx_t j = d; j <= m; j += d) e[j] += tri;
    }
    t[0] = 1;
    for (idx_t k = 1; k <= m; k++) {
        idx_t jmax = m / k;
        /* cb[j] = binomial(e+j-1, j), computed without overflowing intermediates */
        cb[0] = 1;
        for (idx_t j = 1; j <= jmax; j++) {
            seq_t prev = cb[j - 1];
            seq_t num = e[k] + (seq_t)(j - 1);
            seq_t sj = (seq_t)j;
            seq_t g = gcd(prev, sj);
            if (g == 0) g = 1;
            seq_t q = sj / g;
            cb[j] = (prev / g) * (num / q);
        }
        /* in place: t[i] = sum_j cb[j] * t_old[i - k*j], descending i */
        for (idx_t i = m; i >= k; i--) {
            seq_t s = t[i];
            idx_t j = 1;
            for (idx_t r = i - k;; r -= k, j++) {
                s += cb[j] * t[r];
                if (r < k) break;
            }
            t[i] = s;
        }
    }
    mem_free(e);
    mem_free(cb);
    if (A301873_tab) mem_free(A301873_tab);
    A301873_tab = t;
    A301873_cnt = m + 1;
}

seq_t a(idx_t n) {
    if (n >= A301873_cnt) {
        idx_t m = n;
        if (m < 64) m = 64;
        if (m < 2 * A301873_cnt && 2 * A301873_cnt <= 4096) m = 2 * A301873_cnt;
        A301873_build(m);
    }
    return A301873_tab[n];
}
