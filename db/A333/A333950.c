/*
 * A333950  Odd recursive abundant numbers: odd numbers k such that A333926(k)
 *          > 2*k.
 * https://oeis.org/A333950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd k with A333926(k) > 2*k, where A333926(k) =
 *            Product_{p^e || k} (1 + Sum_{d recursive divisor of e} p^d);
 *            computed by a sieve search, doubling the limit
 * Range:     u8 none, u16 n <= 87, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 56 terms through n = 9848 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#define A333950_MAXE 66

static bool A333950_marks[A333950_MAXE + 1][A333950_MAXE + 1];
static bool A333950_init_done = false;

static void A333950_init(void)
{
    if (A333950_init_done) return;
    for (int k = 1; k <= A333950_MAXE; ++k) {
        for (int d = 1; d <= k; ++d) A333950_marks[k][d] = false;
        for (int i = 1; i < k; ++i)
            for (int d = 1; d <= i; ++d)
                if (A333950_marks[i][d] && (k % d == 0))
                    A333950_marks[k][d] = true;
        A333950_marks[k][k] = true;
    }
    A333950_init_done = true;
}

static uint64_t A333950_pp(uint64_t p, int e)
{
    uint64_t sum = 1;
    uint64_t pw = 1;
    for (int d = 1; d <= e; ++d) {
        pw *= p;
        if (A333950_marks[e][d]) sum += pw;
    }
    return sum;
}

static uint64_t *A333950_terms = NULL;
static size_t A333950_cnt = 0;
static size_t A333950_cap = 0;
static uint64_t A333950_limit = 0;

static void A333950_push(uint64_t k)
{
    if (A333950_cnt == A333950_cap) {
        A333950_cap = A333950_cap ? A333950_cap * 2 : 1024;
        A333950_terms = (uint64_t *)realloc(A333950_terms, A333950_cap * sizeof(uint64_t));
    }
    A333950_terms[A333950_cnt++] = k;
}

static void A333950_scan(uint64_t L)
{
    size_t half = (size_t)(L / 2) + 1;
    uint32_t *spf = (uint32_t *)calloc(half, sizeof(uint32_t));
    for (uint64_t p = 3; p * p <= L; p += 2) {
        if (spf[p / 2] == 0) {
            for (uint64_t m = p * p; m <= L; m += 2 * p)
                if (spf[m / 2] == 0) spf[m / 2] = (uint32_t)p;
        }
    }
    A333950_cnt = 0;
    for (uint64_t k = 3; k <= L; k += 2) {
        uint64_t m = k;
        uint64_t prod = 1;
        while (m > 1) {
            uint64_t p = spf[m / 2];
            if (p == 0) p = m;
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            prod *= A333950_pp(p, e);
        }
        if (prod > 2 * k) A333950_push(k);
    }
    free(spf);
    A333950_limit = L;
}

seq_t a(idx_t n)
{
    A333950_init();
    if (n < 1) return 0;
    while (A333950_cnt < (size_t)n || A333950_limit == 0) {
        uint64_t L = A333950_limit ? A333950_limit * 2 : 40000;
        A333950_scan(L);
    }
    return (seq_t)A333950_terms[n - 1];
}
