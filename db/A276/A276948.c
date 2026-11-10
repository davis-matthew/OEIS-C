/*
 * A276948  First differences of A273670: a(n) = A273670(n) - A273670(n-1).
 * https://oeis.org/A276948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A273670(n) - A273670(n-1); A273670(0)=1, then each term
 *            is the next integer > previous with a maximal digit in factorial
 *            base (computed by search)
 * Range:     u8 n <= 260, u16 n <= 5040, u32 n <= 5040, u64 n <= 5040,
 *            u128 n <= 5040, big any n
 * Validated: standard, pass, 136 terms through n = 5009 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool A273670_has_maximal_digit(seq_t x)
{
    seq_t n = x;
    seq_t k = 1;
    while (n > 0) {
        seq_t mod = k + 1;
        seq_t d = n % mod;
        if (d == k) return true;
        n = n / mod;
        k = k + 1;
    }
    return false;
}

static seq_t *A273670_cache = NULL;
static idx_t  A273670_csz = 0;
static idx_t  A273670_cap = 0;

static void A273670_grow(idx_t need)
{
    if (need <= A273670_cap) return;
    idx_t newcap = A273670_cap ? A273670_cap << 1 : 32;
    while (newcap < need) newcap <<= 1;
    seq_t *p = mem_resize(seq_t, A273670_cache, (size_t)newcap);
    if (!p) abort();
    A273670_cache = p;
    A273670_cap = newcap;
}

static void A273670_ensure(idx_t n)
{
    if (n < A273670_csz) return;
    if (!A273670_cache) {
        A273670_grow(32);
        A273670_cache[0] = (seq_t)1;
        A273670_csz = 1;
    }
    while (A273670_csz <= n) {
        seq_t cand = A273670_cache[A273670_csz - 1] + 1;
        while (!A273670_has_maximal_digit(cand)) cand = cand + 1;
        A273670_grow(A273670_csz + 1);
        A273670_cache[A273670_csz++] = cand;
    }
}

seq_t a(idx_t n)
{
    A273670_ensure(n);
    return A273670_cache[n] - A273670_cache[n - 1];
}
