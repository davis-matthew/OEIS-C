/*
 * A289355  Complement of A229289.
 * https://oeis.org/A289355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime not in A229289 (search: test each prime p; p
 *            is in A229289 iff p-1 = 2^k*m, k<=2, m odd squarefree, all prime
 *            factors of m in A229289)
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 945 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static bool A289355_contains(const seq_t *arr, size_t len, seq_t x)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid] < x) lo = mid + 1;
        else if (x < arr[mid]) hi = mid;
        else return true;
    }
    return false;
}

static bool A289355_isprime(seq_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (seq_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

static bool A289355_qualifies(seq_t p, const seq_t *q, size_t qlen)
{
    seq_t t = p - 1;
    unsigned e = 0;
    while (t > 0 && (t & 1) == 0) {
        t >>= 1;
        if (++e > 2) return false;
    }
    seq_t m = t;
    if (m == 1) return true;
    for (seq_t d = 3; d * d <= m; d += 2) {
        if (m % d == 0) {
            m /= d;
            if (m % d == 0) return false;
            if (!A289355_contains(q, qlen, d)) return false;
        }
    }
    if (m > 1 && !A289355_contains(q, qlen, m)) return false;
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *comp = NULL, *qual = NULL;
    static size_t comp_len = 0, comp_cap = 0, qual_len = 0, qual_cap = 0;
    static seq_t cand = 2;

    while (comp_len < n) {
        if (A289355_isprime(cand)) {
            if (A289355_qualifies(cand, qual, qual_len)) {
                if (qual_len + 1 > qual_cap) {
                    size_t nc = qual_cap ? qual_cap * 2 : 64;
                    qual = mem_resize(seq_t, qual, nc);
                    qual_cap = nc;
                }
                qual[qual_len++] = cand;
            } else {
                if (comp_len + 1 > comp_cap) {
                    size_t nc = comp_cap ? comp_cap * 2 : 64;
                    comp = mem_resize(seq_t, comp, nc);
                    comp_cap = nc;
                }
                comp[comp_len++] = cand;
            }
        }
        cand += (cand == 2) ? 1 : 2;
    }
    return comp[n - 1];
}
