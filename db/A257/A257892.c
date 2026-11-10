/*
 * A257892  Numbers n with property that A062234(n) = A062234(n+1) =
 *          A062234(n+2) = A062234(n+3).
 * https://oeis.org/A257892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th index i such that A062234 has a maximal run
 *            of exactly 4 equal values starting at i, i.e. A062234(i-1) !=
 *            A062234(i) = A062234(i+1) = A062234(i+2) = A062234(i+3) !=
 *            A062234(i+4) (equivalently a(n) = A258432(m) where A258383(m) =
 *            4)
 * Range:     u8 none, u16 n <= 22, u32 n <= 110, u64 n <= 110, u128 n <= 110,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 103 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *A257892_pr = NULL;
static size_t A257892_pc = 0;
static uint64_t A257892_lim = 0;

static void A257892_sieve(uint64_t limit)
{
    unsigned char *s = (unsigned char *)calloc((size_t)limit + 1, 1);
    if (!s) abort();
    free(A257892_pr);
    A257892_pr = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)(limit / 2 + 16));
    if (!A257892_pr) abort();
    A257892_pc = 0;
    for (uint64_t i = 2; i <= limit; i++) {
        if (!s[i]) {
            A257892_pr[A257892_pc++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= limit; j += i) s[j] = 1;
        }
    }
    free(s);
    A257892_lim = limit;
}

static void A257892_need(size_t cnt)
{
    while (A257892_lim == 0 || A257892_pc < cnt) {
        uint64_t l = A257892_lim ? A257892_lim * 2 : ((uint64_t)1 << 22);
        A257892_sieve(l);
    }
}

static int64_t A257892_v(size_t i) /* A062234(i), i>=1 */
{
    return 2 * (int64_t)A257892_pr[i - 1] - (int64_t)A257892_pr[i];
}

static uint64_t *A257892_terms = NULL;
static size_t A257892_tc = 0, A257892_tcap = 0;
static uint64_t A257892_next = 1;

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (A257892_tc < (size_t)n) {
        size_t i = (size_t)A257892_next;
        A257892_need(i + 6);
        int64_t v0 = A257892_v(i);
        if ((i == 1 || A257892_v(i - 1) != v0) &&
            v0 == A257892_v(i + 1) && v0 == A257892_v(i + 2) &&
            v0 == A257892_v(i + 3) && A257892_v(i + 4) != v0) {
            if (A257892_tc >= A257892_tcap) {
                size_t nc = A257892_tcap ? A257892_tcap * 2 : 64;
                uint64_t *t = (uint64_t *)realloc(A257892_terms, nc * sizeof(uint64_t));
                if (!t) abort();
                A257892_terms = t;
                A257892_tcap = nc;
            }
            A257892_terms[A257892_tc++] = i;
        }
        A257892_next++;
    }
    return (seq_t)A257892_terms[n - 1];
}
