/*
 * A323157  Lexicographically earliest sequence such that for all i, j, a(i) =
 *          a(j) => f(i) = f(j), where f(n) = A000265(A323155(n)) for all n,
 *          except with f(1) = 0.
 * https://oeis.org/A323157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted-growth (lexicographically earliest) labelling
 *            of f(n), where f(1) = 0 and for n > 1 f(n) = A000265(A323155(n))
 *            = Product_{odd prime p, (p+1)|n} p^(1+v_p(n)). By unique
 *            factorization f(n) is fully determined by the sorted list of
 *            pairs (p, 1+v_p(n)), so the code compares those li...
 * Range:     u8 n <= 869, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9743 (2026-11-08)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static uint64_t *A323157_col = NULL;
static idx_t A323157_N = 0;

static void A323157_add(idx_t n, idx_t dd, uint64_t *buf, size_t *k)
{
    if (dd <= 3) return;               /* need odd prime p = dd-1 >= 3 */
    idx_t p = dd - 1;
    if ((p & 1) == 0 || !is_prime(p)) return;
    idx_t tmp = n, v = 0;
    while (tmp % p == 0) { tmp /= p; ++v; }
    buf[2 * (*k)] = (uint64_t)p;
    buf[2 * (*k) + 1] = (uint64_t)(v + 1);
    ++(*k);
}

static size_t A323157_sig(idx_t n, uint64_t *buf)
{
    size_t k = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;
        A323157_add(n, d, buf, &k);
        if (n / d != d) A323157_add(n, n / d, buf, &k);
    }
    /* insertion sort pairs by p */
    for (size_t i = 1; i < k; ++i) {
        uint64_t p = buf[2 * i], e = buf[2 * i + 1];
        size_t j = i;
        while (j > 0 && buf[2 * (j - 1)] > p) {
            buf[2 * j] = buf[2 * (j - 1)];
            buf[2 * j + 1] = buf[2 * (j - 1) + 1];
            --j;
        }
        buf[2 * j] = p;
        buf[2 * j + 1] = e;
    }
    return k;
}

static uint64_t A323157_hash(const uint64_t *b, size_t k)
{
    uint64_t h = 1469598103934665603ULL ^ (uint64_t)k;
    for (size_t i = 0; i < 2 * k; ++i) {
        h ^= b[i];
        h *= 1099511628211ULL;
        h ^= h >> 29;
    }
    return h;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n > A323157_N) {
        idx_t N = n;
        if (N < 1024) N = 1024;
        if (N < 2 * A323157_N) N = 2 * A323157_N;
        free(A323157_col);
        A323157_col = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(N + 1));

        /* divisor count bound: 2*sqrt(N)+2 */
        idx_t s = 1;
        while ((s + 1) <= N / (s + 1)) ++s;
        size_t maxk = (size_t)(2 * s + 4);
        uint64_t *buf = (uint64_t *)malloc(sizeof(uint64_t) * 2 * maxk);

        /* class storage */
        size_t cap = 1024, used = 0;
        uint64_t *store = (uint64_t *)malloc(sizeof(uint64_t) * cap);
        size_t *off = (size_t *)malloc(sizeof(size_t) * (size_t)(N + 2));
        size_t *len = (size_t *)malloc(sizeof(size_t) * (size_t)(N + 2));
        size_t tsz = 1;
        while (tsz < 2 * (size_t)N + 2) tsz <<= 1;
        uint64_t *table = (uint64_t *)calloc(tsz, sizeof(uint64_t)); /* class id, 0 = empty */

        uint64_t cnt = 1;                 /* class 1 is f(1) = 0 */
        A323157_col[0] = 0;
        A323157_col[1] = 1;
        for (idx_t m = 2; m <= N; ++m) {
            size_t k = A323157_sig(m, buf);
            uint64_t h = A323157_hash(buf, k);
            size_t pos = (size_t)h & (tsz - 1);
            uint64_t found = 0;
            while (table[pos] != 0) {
                uint64_t c = table[pos];
                if (len[c] == k) {
                    const uint64_t *q = store + off[c];
                    size_t i = 0;
                    while (i < 2 * k && q[i] == buf[i]) ++i;
                    if (i == 2 * k) { found = c; break; }
                }
                pos = (pos + 1) & (tsz - 1);
            }
            if (!found) {
                ++cnt;
                found = cnt;
                if (used + 2 * k > cap) {
                    while (used + 2 * k > cap) cap *= 2;
                    store = (uint64_t *)realloc(store, sizeof(uint64_t) * cap);
                }
                for (size_t i = 0; i < 2 * k; ++i) store[used + i] = buf[i];
                off[found] = used;
                len[found] = k;
                used += 2 * k;
                table[pos] = found;
            }
            A323157_col[m] = found;
        }
        free(buf);
        free(store);
        free(off);
        free(len);
        free(table);
        A323157_N = N;
    }
    return (seq_t)A323157_col[n];
}
