/*
 * A319523  Square array T(n, k) (n >= 1, k >= 1) read by antidiagonals
 *          upwards: T(n, k) is the unique positive integer m such that
 *          A319521(m) = n and A319522(m) = k.
 * https://oeis.org/A319523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = A319525(n) * A297002(k), read by antidiagonals upwards.
 *            A319525: completely multiplicative, prime(j) -> prime(2j-1).
 *            A297002: completely multiplicative, prime(j) -> prime(2j). Index
 *            m = i + d(d-1)/2 with 1<=i<=d gives n = d+1-i, k = i
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static uint32_t *T_pr = NULL;
static size_t T_sz = 0, T_cap = 0;

static void T_extend(void)
{
    if (T_sz == 0) {
        T_cap = 64;
        T_pr = (uint32_t *)malloc(T_cap * sizeof(uint32_t));
        if (!T_pr) abort();
        T_pr[T_sz++] = 2;
        return;
    }
    uint32_t c = T_pr[T_sz - 1] + 1;
    if (c > 2) c |= 1;
    for (;; c += 2) {
        bool ok = true;
        for (size_t i = 0; i < T_sz; ++i) {
            uint64_t q = T_pr[i];
            if (q * q > c) break;
            if (c % q == 0) { ok = false; break; }
        }
        if (ok) break;
    }
    if (T_sz == T_cap) {
        T_cap <<= 1;
        T_pr = (uint32_t *)realloc(T_pr, T_cap * sizeof(uint32_t));
        if (!T_pr) abort();
    }
    T_pr[T_sz++] = c;
}

/* k-th prime, k>=1 */
static uint32_t T_prime(size_t k)
{
    if (T_sz == 0) T_extend();
    while (T_sz < k) T_extend();
    return T_pr[k - 1];
}

/* completely multiplicative map prime(j) -> prime(2j - off), off in {0,1} */
static seq_t T_map(uint64_t n, int off)
{
    seq_t res = 1;
    if (n <= 1) return res;
    uint64_t rem = n;
    for (uint64_t p = 2; p * p <= rem; p += (p == 2 ? 1 : 2)) {
        if (rem % p) continue;
        uint64_t e = 0;
        while (rem % p == 0) { rem /= p; ++e; }
        /* index of p */
        size_t j = 1;
        while (T_prime(j) != p) ++j;
        seq_t base = (seq_t)T_prime(2 * j - off);
        while (e--) res *= base;
    }
    if (rem > 1) {
        size_t j = 1;
        while (T_prime(j) < rem) ++j;
        res *= (seq_t)T_prime(2 * j - off);
    }
    return res;
}

seq_t a(idx_t m)
{
    uint64_t mm = (uint64_t)m;
    uint64_t d = (uint64_t)(((double)(-1.0) + __builtin_sqrt(8.0 * (double)mm + 1.0)) / 2.0);
    while (d > 0 && d * (d + 1) / 2 >= mm) --d;
    while (d * (d + 1) / 2 < mm) ++d;
    uint64_t i = mm - d * (d - 1) / 2;
    uint64_t n = d + 1 - i, k = i;
    return T_map(n, 1) * T_map(k, 0);
}
