/*
 * A354867  Sum of A122111 and its Dirichlet inverse.
 * https://oeis.org/A354867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A122111(n) + A354866(n), where A354866 is the Dirichlet
 *            inverse of A122111: b(1) = 1, b(n) = -Sum_{d|n, d>1}
 *            A122111(d)*b(n/d); so a(1) = 2 and a(n) = -Sum_{d|n, 1<d<n}
 *            A122111(d)*A354866(n/d) for n > 1
 * Range:     i8 n <= 20, i16 n <= 81, i32 n <= 217, i64 n <= 565,
 *            i128 n <= 1381, big any n
 * Validated: standard, pass, 93 terms through n = 7790 (2026-11-10)
 * Helpers:   factor_u64, ipow_idx, mem, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"
#include "../utils/prime_pi.h"
#include "../utils/mem.h"

static uint32_t A122111_get_prime(idx_t k)
{
    static uint32_t *pr = NULL;
    static size_t sz   = 0;
    static size_t cap  = 0;
    static uint32_t cand = 3;

    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    while (sz < (size_t)k) {
        if (sz == 0) {
            pr[sz++] = 2;
            continue;
        }
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }
        if (!ok) {
            cand += 2;
            continue;
        }
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }
    return pr[(size_t)k - 1];
}

static int A122111_cmp_idx(const void *a, const void *b)
{
    idx_t va = *(const idx_t *)a;
    idx_t vb = *(const idx_t *)b;
    return (va > vb) - (va < vb);
}

static seq_t A122111_a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt;
    factor_u64(n, primes, exps, &pcnt);

    idx_t indices[128];
    size_t idx_cnt = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        idx_t idx = (idx_t)prime_pi(primes[i]);
        for (uint64_t e = 0; e < exps[i]; ++e) {
            indices[idx_cnt++] = idx;
        }
    }

    if (idx_cnt == 0) return (seq_t)1;

    qsort(indices, idx_cnt, sizeof(idx_t), A122111_cmp_idx);

    seq_t result = (seq_t)1;
    idx_t remaining = idx_cnt;
    idx_t last_part = 1;
    size_t pos = 0;
    while (pos < idx_cnt) {
        idx_t current = indices[pos];
        size_t freq = 0;
        while (pos < idx_cnt && indices[pos] == current) {
            ++freq;
            ++pos;
        }
        idx_t length = current - last_part + 1;
        seq_t prime_val = (seq_t)A122111_get_prime(remaining);
        seq_t term = ipow_idx(prime_val, length);
        result = result * term;
        remaining -= freq;
        last_part = current + 1;
    }
    return result;
}

static size_t A354867_find(const idx_t *d, size_t cnt, idx_t v)
{
    size_t lo = 0, hi = cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (d[mid] < v) lo = mid + 1; else hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* enumerate divisors */
    size_t cap = 64, cnt = 0;
    idx_t *d = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!d) abort();
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            if (cnt + 2 > cap) {
                cap *= 2;
                d = (idx_t *)realloc(d, cap * sizeof(idx_t));
                if (!d) abort();
            }
            d[cnt++] = i;
            if (i != n / i) d[cnt++] = n / i;
        }
    }
    qsort(d, cnt, sizeof(idx_t), A122111_cmp_idx);

    seq_t *f = mem_new(seq_t, cnt);
    seq_t *b = mem_new(seq_t, cnt);
    for (size_t i = 0; i < cnt; ++i) f[i] = A122111_a(d[i]);

    b[0] = (seq_t)1;
    for (size_t i = 1; i < cnt; ++i) {
        idx_t m = d[i];
        seq_t s = (seq_t)0;
        for (size_t j = 1; j <= i; ++j) {
            if (m % d[j] == 0) {
                size_t k = A354867_find(d, cnt, m / d[j]);
                s += f[j] * b[k];
            }
        }
        b[i] = -s;
    }
    seq_t res = f[cnt - 1] + b[cnt - 1];

    mem_free(f);
    mem_free(b);
    free(d);
    return res;
}
