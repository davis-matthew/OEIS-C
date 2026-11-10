/*
 * A318310  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A000120(i) = A000120(j) and A033879(i) = A033879(j), for all i,
 *          j >= 0.
 * https://oeis.org/A318310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   restricted growth sequence transform of the ordered pair
 *            [A000120(n), A033879(n)] = [popcount(n), 2n - sigma(n)]: a(n) is
 *            the 1-based rank of the first index k <= n with the same pair,
 *            ranked by first occurrence. Computed by a divisor-sum sieve
 *            followed by sorting the pairs
 * Range:     u8 n <= 322, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 110 terms through n = 9397 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint64_t pop;
    int64_t def;
    uint64_t idx;
} A318310_item;

static int A318310_cmp(const void *x, const void *y)
{
    const A318310_item *p = (const A318310_item *)x;
    const A318310_item *q = (const A318310_item *)y;
    if (p->pop != q->pop) return p->pop < q->pop ? -1 : 1;
    if (p->def != q->def) return p->def < q->def ? -1 : 1;
    if (p->idx != q->idx) return p->idx < q->idx ? -1 : 1;
    return 0;
}

static uint32_t *A318310_ids = 0;
static uint64_t A318310_limit = 0;

static void A318310_build(uint64_t L)
{
    uint64_t i, j;
    uint64_t *sig = (uint64_t *)calloc((size_t)(L + 1), sizeof(uint64_t));
    A318310_item *it = (A318310_item *)malloc((size_t)L * sizeof(A318310_item));
    uint64_t *first = (uint64_t *)calloc((size_t)(L + 1), sizeof(uint64_t));
    uint32_t *ids = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    uint32_t cnt = 0;

    for (i = 1; i <= L; i++)
        for (j = i; j <= L; j += i)
            sig[j] += i;
    for (i = 1; i <= L; i++) {
        it[i - 1].pop = (uint64_t)__builtin_popcountll((unsigned long long)i);
        it[i - 1].def = (int64_t)(2 * i) - (int64_t)sig[i];
        it[i - 1].idx = i;
    }
    qsort(it, (size_t)L, sizeof(A318310_item), A318310_cmp);
    {
        uint64_t f = 0;
        for (i = 0; i < L; i++) {
            if (i == 0 || it[i].pop != it[i - 1].pop || it[i].def != it[i - 1].def)
                f = it[i].idx;
            first[it[i].idx] = f;
        }
    }
    for (i = 1; i <= L; i++) {
        if (first[i] == i) ids[i] = ++cnt;
        else ids[i] = ids[first[i]];
    }
    free(sig);
    free(it);
    free(first);
    if (A318310_ids) free(A318310_ids);
    A318310_ids = ids;
    A318310_limit = L;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if ((uint64_t)n > A318310_limit) {
        uint64_t L = (uint64_t)n;
        if (L < 2 * A318310_limit) L = 2 * A318310_limit;
        if (L < 1024) L = 1024;
        A318310_build(L);
    }
    return (seq_t)A318310_ids[(uint64_t)n];
}
