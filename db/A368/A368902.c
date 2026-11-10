/*
 * A368902  Numbers k for which A342001(k) is squarefree.
 * https://oeis.org/A368902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 such that A342001(k) is squarefree, where
 *            A342001(k) = Sum_{p^v || k} v*rad(k)/p; computed by search
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 79 terms through n = 79 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t A342001_a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    idx_t t = n;
    idx_t ps[64];
    idx_t es[64];
    int fc = 0;
    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            idx_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            ps[fc] = p;
            es[fc] = e;
            ++fc;
        }
    }
    if (t > 1) {
        ps[fc] = t;
        es[fc] = 1;
        ++fc;
    }
    seq_t rad = (seq_t)1;
    for (int i = 0; i < fc; ++i)
        rad *= (seq_t)ps[i];
    seq_t res = (seq_t)0;
    for (int i = 0; i < fc; ++i)
        res += (seq_t)es[i] * (rad / (seq_t)ps[i]);
    return res;
}

static bool A368902_sqfree(seq_t v)
{
    if (v == 0)
        return false;
    for (seq_t d = 2; d * d <= v; ++d) {
        if (v % (d * d) == 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    static idx_t *cache = 0;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;

    if (n > cache_size) {
        if (n > cache_cap) {
            idx_t newcap = cache_cap ? cache_cap : 64;
            while (newcap < n)
                newcap *= 2;
            cache = (idx_t *)realloc(cache, (size_t)newcap * sizeof(idx_t));
            cache_cap = newcap;
        }
        idx_t k = cache_size ? cache[cache_size - 1] : 1;
        for (idx_t i = cache_size; i < n; i++) {
            do {
                k++;
            } while (!A368902_sqfree(A342001_a(k)));
            cache[i] = k;
        }
        cache_size = n;
    }
    return (seq_t)cache[n - 1];
}
