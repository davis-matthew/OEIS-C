/*
 * A324532  Lexicographically earliest sequence such that a(i) = a(j) => f(i)
 *          = f(j), where f(n) = [A000120(n), A318458(n)] for all other
 *          numbers, except f(1) = 0.
 * https://oeis.org/A324532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rank (by first appearance) of f(n) = [A000120(n), n AND
 *            (sigma(n)-n)], with f(1)=0; computed by sieve + sort
 * Range:     u8 n <= 519, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 117 terms through n = 8781 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

typedef struct { uint64_t key; uint64_t idx; } A324532_pair;

static int A324532_cmp(const void *x, const void *y)
{
    const A324532_pair *p = (const A324532_pair *)x;
    const A324532_pair *q = (const A324532_pair *)y;
    if (p->key != q->key) return p->key < q->key ? -1 : 1;
    if (p->idx != q->idx) return p->idx < q->idx ? -1 : 1;
    return 0;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    if (N < 1) return 0;
    uint64_t *prop = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    A324532_pair *arr = (A324532_pair *)malloc(N * sizeof(A324532_pair));
    unsigned char *isfirst = (unsigned char *)calloc(N + 1, 1);
    uint64_t *keys = (uint64_t *)malloc((N + 1) * sizeof(uint64_t));
    size_t d, k, m;
    for (d = 1; d <= N / 2; ++d)
        for (k = 2 * d; k <= N; k += d) prop[k] += d;
    for (m = 1; m <= N; ++m) {
        uint64_t key;
        if (m == 1) key = 0;
        else {
            uint64_t pc = 0, x = m;
            while (x) { pc += x & 1; x >>= 1; }
            key = (pc << 40) | ((uint64_t)m & prop[m]);
        }
        keys[m] = key;
        arr[m - 1].key = key;
        arr[m - 1].idx = m;
    }
    qsort(arr, N, sizeof(A324532_pair), A324532_cmp);
    for (m = 0; m < N; ++m)
        if (m == 0 || arr[m].key != arr[m - 1].key) isfirst[arr[m].idx] = 1;
    uint64_t fi = 0;
    for (m = 0; m < N; ++m)
        if (arr[m].key == keys[N]) { fi = arr[m].idx; break; }
    uint64_t rank = 0;
    for (m = 1; m <= (size_t)fi; ++m) rank += isfirst[m];
    free(prop); free(arr); free(isfirst); free(keys);
    return (seq_t)rank;
}
