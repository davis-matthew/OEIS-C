/*
 * A342022  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A342002(i) = A342002(j) for all i, j >= 0.
 * https://oeis.org/A342022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = restricted growth transform of A342002: a(n) = number of
 *            distinct values among A342002(0..m), where m is the first index
 *            with A342002(m) = A342002(n). Computed by direct evaluation plus
 *            sorting
 * Range:     u8 n <= 53, u16 n <= 3066, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 100 terms through n = 9221 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

static const unsigned A342022_primes[] = {
    2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u, 23u, 29u,
    31u, 37u, 41u, 43u, 47u, 53u, 59u, 61u, 67u, 71u,
    73u, 79u, 83u, 89u, 97u, 101u, 103u, 107u, 109u, 113u
};
#define A342022_PC (sizeof(A342022_primes)/sizeof(A342022_primes[0]))

static seq_t A342002_a(idx_t n)
{
    if (n == 0) return 0;
    idx_t tmp = n;
    seq_t rad = 1;
    unsigned digits[64] = {0};
    size_t idx = 0;
    while (tmp > 0 || idx == 0) {
        if (idx >= A342022_PC) break;
        unsigned p = A342022_primes[idx];
        unsigned d = (unsigned)(tmp % p);
        digits[idx] = d;
        tmp = tmp / p;
        if (d > 0) rad *= (seq_t)p;
        ++idx;
    }
    seq_t result = 0;
    for (size_t j = 0; j < idx; ++j) {
        unsigned d = digits[j];
        if (d == 0) continue;
        seq_t term = (rad / (seq_t)A342022_primes[j]) * (seq_t)d;
        result += term;
    }
    return result;
}

static void A342022_msort(seq_t *a, seq_t *t, size_t n)
{
    for (size_t w = 1; w < n; w *= 2) {
        for (size_t lo = 0; lo < n; lo += 2 * w) {
            size_t mid = lo + w < n ? lo + w : n;
            size_t hi = lo + 2 * w < n ? lo + 2 * w : n;
            size_t i = lo, j = mid, k = lo;
            while (i < mid && j < hi) {
                if (a[j] < a[i]) t[k++] = a[j++];
                else t[k++] = a[i++];
            }
            while (i < mid) t[k++] = a[i++];
            while (j < hi) t[k++] = a[j++];
            for (size_t q = lo; q < hi; ++q) a[q] = t[q];
        }
    }
}

seq_t a(idx_t n)
{
    size_t len = (size_t)n + 1;
    seq_t *v = mem_new(seq_t, len);
    for (size_t i = 0; i < len; ++i) v[i] = A342002_a((idx_t)i);

    size_t m = 0;
    while (!(v[m] == v[len - 1])) ++m;

    size_t cnt = m + 1;
    seq_t *s = mem_new(seq_t, cnt);
    seq_t *t = mem_new(seq_t, cnt);
    for (size_t i = 0; i < cnt; ++i) s[i] = v[i];
    A342022_msort(s, t, cnt);
    size_t d = 1;
    for (size_t i = 1; i < cnt; ++i) {
        if (!(s[i] == s[i - 1])) ++d;
    }
    mem_free(s);
    mem_free(t);
    mem_free(v);
    return (seq_t)d;
}
