/*
 * A246203  Permutation of natural numbers: a(n) = A246201(A193231(n)).
 * https://oeis.org/A246203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A246201(A193231(n)); A193231 is the substitution x -> x+1
 *            on GF(2) polynomial codes, and A246201 is computed by a sieve
 *            with a(1) = 1, a(k-th irreducible) = 2*a(k)+1, a(k-th reducible)
 *            = 2*a(k)
 * Range:     u8 n <= 38, u16 n <= 138, u32 n <= 2055, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 91 terms through n = 9391 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static inline int A246203_deg(uint32_t p) {
    return 31 - __builtin_clz(p);
}

static inline uint32_t A246203_mod(uint32_t f, uint32_t g) {
    int dg = A246203_deg(g);
    while (f != 0) {
        int df = A246203_deg(f);
        if (df < dg) break;
        f ^= g << (df - dg);
    }
    return f;
}

static seq_t *A246203_cache = NULL;
static size_t A246203_len = 0, A246203_cap = 0;
static uint32_t *A246203_irr = NULL;
static size_t A246203_irr_len = 0, A246203_irr_cap = 0;
static size_t A246203_ic = 0, A246203_rc = 0;

static void A246203_ensure(size_t idx) {
    if (idx + 1 <= A246203_len) return;
    size_t need = idx + 1;
    if (need > A246203_cap) {
        size_t nc = A246203_cap ? A246203_cap * 2 : 16;
        if (nc < need) nc = need + 16;
        A246203_cache = mem_resize(seq_t, A246203_cache, nc);
        A246203_cap = nc;
    }
    if (A246203_irr == NULL) {
        A246203_irr_cap = 16;
        A246203_irr = (uint32_t *)malloc(A246203_irr_cap * sizeof(uint32_t));
    }
    if (A246203_len == 0) {
        A246203_cache[0] = 0;
        A246203_cache[1] = 1;
        A246203_len = 2;
    }
    for (size_t i = A246203_len; i < need; ++i) {
        uint32_t val = (uint32_t)i;
        bool irreducible = true;
        int maxd = A246203_deg(val) >> 1;
        for (size_t j = 0; j < A246203_irr_len; ++j) {
            uint32_t q = A246203_irr[j];
            if (A246203_deg(q) > maxd) break;
            if (A246203_mod(val, q) == 0) { irreducible = false; break; }
        }
        seq_t ak;
        if (irreducible) {
            ++A246203_ic;
            ak = A246203_cache[A246203_ic];
            ak = ak << 1;
            ak += (seq_t)1;
            A246203_cache[i] = ak;
            if (A246203_irr_len == A246203_irr_cap) {
                A246203_irr_cap = A246203_irr_cap * 2 + 16;
                A246203_irr = (uint32_t *)realloc(A246203_irr, A246203_irr_cap * sizeof(uint32_t));
            }
            A246203_irr[A246203_irr_len++] = val;
        } else {
            ++A246203_rc;
            ak = A246203_cache[A246203_rc];
            ak = ak << 1;
            A246203_cache[i] = ak;
        }
    }
    A246203_len = need;
}

static uint64_t A246203_blue(uint64_t n) {
    uint64_t res = 0, mask = 1;
    while (n) {
        if (n & 1) res ^= mask;
        mask = (mask << 1) ^ mask;
        n >>= 1;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    size_t m = (size_t)A246203_blue((uint64_t)n);
    A246203_ensure(m);
    return A246203_cache[m];
}
