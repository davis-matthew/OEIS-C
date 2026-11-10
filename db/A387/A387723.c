/*
 * A387723  Numbers k for which A107758(k) > 2*k, and also for some of the
 *          proper divisors d|k, d<k, A107758(d) > 2*d.
 * https://oeis.org/A387723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A107758(k) > 2k and some proper divisor d|k,
 *            d<k, with A107758(d) > 2d (search with sieve)
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9650 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *t_terms = NULL;
static size_t t_cnt = 0;
static uint64_t t_limit = 0;

static void t_build(uint64_t L)
{
    uint64_t *g = (uint64_t*)malloc((L + 1) * sizeof(uint64_t));
    unsigned char *comp = (unsigned char*)calloc(L + 1, 1);
    unsigned char *hd = (unsigned char*)calloc(L + 1, 1);
    uint64_t i, m;
    if (!g || !comp || !hd) { free(g); free(comp); free(hd); return; }
    for (i = 0; i <= L; i++) g[i] = 1;
    for (i = 2; i <= L; i++) {
        if (comp[i]) continue;
        for (m = i * i; m <= L; m += i) comp[m] = 1;
        for (m = i; m <= L; m += i) {
            uint64_t t = m, term = 2, pw = 1;
            while (t % i == 0) { t /= i; pw *= i; term += pw; }
            g[m] *= term;
        }
    }
    for (i = 2; i <= L; i++)
        if (g[i] > 2 * i)
            for (m = 2 * i; m <= L; m += i) hd[m] = 1;
    free(t_terms);
    t_terms = (uint64_t*)malloc((L + 1) * sizeof(uint64_t));
    t_cnt = 0;
    if (t_terms) {
        for (i = 2; i <= L; i++)
            if (hd[i] && g[i] > 2 * i) t_terms[t_cnt++] = i;
    }
    t_limit = L;
    free(g); free(comp); free(hd);
}

seq_t a(idx_t n)
{
    if (t_limit == 0) t_build(1024);
    while (t_cnt < (size_t)n) t_build(t_limit * 2);
    return (seq_t)t_terms[(size_t)n - 1];
}
