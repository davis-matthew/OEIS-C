/*
 * A323156  Lexicographically earliest sequence such that for all i, j, a(i) =
 *          a(j) => f(i) = f(j), where f(n) = A323155(n) for all n, except
 *          f(1) = 0.
 * https://oeis.org/A323156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted-growth index of f(n): label values f(m),
 *            m=1..n, by order of first appearance; f(1)=0, f(n)=A323155(n) =
 *            Product_{d|n, d-1 prime} (d-1)^(1+A286561(n,d-1)) otherwise
 *            (computed by hashing/memoized scan)
 * Range:     u8 n <= 29, u16 n <= 2867, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9596 (2026-11-08)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

static seq_t A323155_a(idx_t n)
{
    seq_t res = 1;
    if (n < 2) return 1;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;
        idx_t ds[2];
        int k = 0;
        ds[k++] = d;
        if (n / d != d) ds[k++] = n / d;
        for (int t = 0; t < k; ++t) {
            idx_t p = ds[t] - 1;
            if (p >= 2 && is_prime(p)) {
                idx_t tmp = n, v = 0;
                while (tmp % p == 0) { tmp /= p; ++v; }
                for (idx_t i = 0; i < v + 1; ++i) res *= (seq_t)p;
            }
        }
    }
    return res;
}

#define A323156_H 16381

static seq_t *A323156_fv = 0;
static idx_t *A323156_id = 0;
static idx_t *A323156_nxt = 0;
static idx_t *A323156_head = 0;
static idx_t A323156_cap = 0;
static idx_t A323156_done = 0;
static idx_t A323156_count = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n > A323156_cap) {
        if (A323156_fv) {
            mem_free(A323156_fv);
            mem_free(A323156_id);
            mem_free(A323156_nxt);
            mem_free(A323156_head);
        }
        idx_t cap = A323156_cap * 2;
        if (cap < n) cap = n;
        if (cap < 1024) cap = 1024;
        A323156_cap = cap;
        A323156_fv = mem_new(seq_t, cap + 1);
        A323156_id = mem_new(idx_t, cap + 1);
        A323156_nxt = mem_new(idx_t, cap + 1);
        A323156_head = mem_new(idx_t, A323156_H);
        A323156_done = 0;
        A323156_count = 0;
    }
    while (A323156_done < n) {
        idx_t m = ++A323156_done;
        seq_t f = (m == 1) ? (seq_t)0 : A323155_a(m);
        A323156_fv[m] = f;
        idx_t h = (idx_t)(unsigned long long)(f % (seq_t)A323156_H);
        idx_t cur = A323156_head[h];
        idx_t found = 0;
        while (cur) {
            if (A323156_fv[cur] == f) { found = cur; break; }
            cur = A323156_nxt[cur];
        }
        if (found) {
            A323156_id[m] = A323156_id[found];
        } else {
            A323156_id[m] = ++A323156_count;
            A323156_nxt[m] = A323156_head[h];
            A323156_head[h] = m;
        }
    }
    return (seq_t)A323156_id[n];
}
