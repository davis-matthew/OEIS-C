/*
 * A305318  Numbers k such that A071866(k)=3.
 * https://oeis.org/A305318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that the continued fraction of
 *            prime(k+1)/prime(k) has exactly 3 terms (A071866(k)=3); found by
 *            incremental search
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9370 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A305318_isp(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A305318_next(uint64_t p)
{
    uint64_t c = (p == 2) ? 3 : p + 2;
    while (!A305318_isp(c)) c += 2;
    return c;
}

static int A305318_cflen(uint64_t num, uint64_t den)
{
    int len = 0;
    while (den != 0) {
        uint64_t r = num % den;
        num = den;
        den = r;
        len++;
    }
    return len;
}

static uint64_t *A305318_cache = NULL;
static size_t A305318_len = 0, A305318_cap = 0;
static uint64_t A305318_k = 1, A305318_p = 2, A305318_q = 3;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A305318_len < (size_t)n) {
        if (A305318_cflen(A305318_q, A305318_p) == 3) {
            if (A305318_len == A305318_cap) {
                size_t nc = A305318_cap ? A305318_cap * 2 : 64;
                uint64_t *t = (uint64_t *)realloc(A305318_cache, nc * sizeof(uint64_t));
                if (!t) abort();
                A305318_cache = t;
                A305318_cap = nc;
            }
            A305318_cache[A305318_len++] = A305318_k;
        }
        A305318_k++;
        A305318_p = A305318_q;
        A305318_q = A305318_next(A305318_q);
    }
    return (seq_t)A305318_cache[(size_t)n - 1];
}
