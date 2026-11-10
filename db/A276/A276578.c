/*
 * A276578  Transpose of square array A255483.
 * https://oeis.org/A276578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Product_{k=0..i} prime(j+k)^[C(i,k) odd], where the
 *            antidiagonal d (d>=1) holds n, offset o = n - d(d-1)/2, i =
 *            d-1-o, j = o+1 (transpose of A255483, T(0,m)=prime(m))
 * Range:     u8 n <= 10, u16 n <= 27, u32 n <= 57, u64 n <= 119,
 *            u128 n <= 209, big any n
 * Validated: standard, pass, 82 terms through n = 207 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static uint32_t A276578_prime(idx_t k)
{
    static uint32_t *pr = NULL;
    static size_t sz = 0;
    static size_t cap = 0;
    static uint32_t cand = 3;

    if (cap == 0) {
        cap = 8;
        pr = (uint32_t *)malloc(cap * sizeof(uint32_t));
        if (!pr) abort();
    }

    while (sz < (size_t)k) {
        if (sz == 0) {
            pr[sz++] = 2;
            continue;
        }
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t q = pr[i];
            if ((uint64_t)q * q > (uint64_t)p) break;
            if (p % q == 0) { ok = false; break; }
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

seq_t a(idx_t n)
{
    idx_t d = 1;
    while ((d * (d + 1) / 2) <= n) ++d;

    idx_t prev = (d - 1) * d / 2;
    idx_t o = n - prev;
    idx_t i = d - 1 - o;
    idx_t j = o + 1;

    seq_t result = 1;
    for (idx_t k = 0; k <= i; ++k) {
        if ((k & (i - k)) == 0) {
            result *= (seq_t)A276578_prime(j + k);
        }
    }
    return result;
}
