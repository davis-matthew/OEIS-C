/*
 * A358105  Unreduced denominator of the n-th divisible pair, where pairs are
 *          ordered by Heinz number. Lesser prime index of A318990(n).
 * https://oeis.org/A358105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(p) where A318990(n) = p*q, p<=q primes, pi(p) | pi(q);
 *            computed by generating and sorting all such products
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint64_t prod;
    uint32_t x;
} a358105_pair;

static int a358105_cmp(const void *p1, const void *p2)
{
    const a358105_pair *u = (const a358105_pair *)p1;
    const a358105_pair *v = (const a358105_pair *)p2;
    if (u->prod < v->prod) return -1;
    if (u->prod > v->prod) return 1;
    return 0;
}

static a358105_pair *a358105_tab = NULL;
static size_t a358105_len = 0;

static void a358105_build(void)
{
    const uint32_t N = 20000;
    const uint32_t LIM = 230000;
    unsigned char *comp = (unsigned char *)calloc(LIM + 1, 1);
    uint32_t *pr = (uint32_t *)malloc(sizeof(uint32_t) * (N + 1));
    if (!comp || !pr) { free(comp); free(pr); return; }
    uint32_t cnt = 0;
    for (uint32_t i = 2; i <= LIM && cnt < N; i++) {
        if (!comp[i]) {
            pr[cnt++] = i;
            for (uint64_t j = (uint64_t)i * i; j <= LIM; j += i) comp[j] = 1;
        }
    }
    free(comp);
    if (cnt < N) { free(pr); return; }

    size_t total = 0;
    for (uint32_t x = 1; x <= N; x++) total += N / x;
    a358105_pair *t = (a358105_pair *)malloc(sizeof(a358105_pair) * total);
    if (!t) { free(pr); return; }
    size_t k = 0;
    for (uint32_t x = 1; x <= N; x++) {
        for (uint32_t y = x; y <= N; y += x) {
            t[k].prod = (uint64_t)pr[x - 1] * pr[y - 1];
            t[k].x = x;
            k++;
        }
    }
    free(pr);
    qsort(t, k, sizeof(a358105_pair), a358105_cmp);
    a358105_tab = t;
    a358105_len = k;
}

seq_t a(idx_t n)
{
    if (a358105_len == 0) a358105_build();
    if (n < 1 || n > a358105_len) return (seq_t)0;
    return (seq_t)a358105_tab[n - 1].x;
}
