/*
 * A279337  a(1) = 1, for n > 1, if n is even, a(n) = A055938(n/2), otherwise
 *          a(n) = A005187(a(A064989(n))).
 * https://oeis.org/A279337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, if n is even, a(n) = A055938(n/2),
 *            otherwise a(n) = A005187(a(A064989(n)))
 * Range:     u8 n <= 28, u16 n <= 60, u32 n <= 138, u64 n <= 316,
 *            u128 n <= 732, big any n
 * Validated: standard, pass, 94 terms through n = 1852 (2026-11-08)
 * Helpers:   factor_u64, is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

/* ---------- A055938 (copied, renamed) ---------- */
static seq_t *A055938_b        = NULL;
static size_t A055938_b_len    = 0, A055938_b_cap   = 0;
static bool *A055938_present   = NULL;
static size_t A055938_present_cap = 0;
static seq_t *A055938_missing  = NULL;
static size_t A055938_missing_len = 0, A055938_missing_cap = 0;
static idx_t A055938_next_cand = 1;

static void A055938_grow_b(size_t need)
{
    if (A055938_b_cap == 0) A055938_b_cap = 16;
    while (A055938_b_cap < need) A055938_b_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, A055938_b, A055938_b_cap);
    if (!tmp) exit(1);
    A055938_b = tmp;
}

static void A055938_grow_missing(size_t need)
{
    if (A055938_missing_cap == 0) A055938_missing_cap = 64;
    while (A055938_missing_cap < need) A055938_missing_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, A055938_missing, A055938_missing_cap);
    if (!tmp) exit(1);
    A055938_missing = tmp;
}

static void A055938_ensure_present_size(seq_t t)
{
    size_t tt = (size_t)t;
    if (tt < A055938_present_cap) return;
    size_t new_cap = A055938_present_cap ? A055938_present_cap : 1;
    while (new_cap <= tt) new_cap <<= 1;
    bool *tmp = (bool *)realloc(A055938_present, new_cap * sizeof(bool));
    if (!tmp) exit(1);
    memset(tmp + A055938_present_cap, 0, (new_cap - A055938_present_cap) * sizeof(bool));
    A055938_present = tmp;
    A055938_present_cap = new_cap;
}

static void A055938_ensure_b_up_to_value(seq_t target)
{
    A055938_ensure_present_size(target);
    if (A055938_b_len == 0) {
        A055938_grow_b(1);
        A055938_b[0] = 0;
        A055938_b_len = 1;
        A055938_ensure_present_size(0);
        A055938_present[0] = true;
    }
    while ((seq_t)A055938_b[A055938_b_len - 1] < target) {
        size_t next_index = A055938_b_len;
        seq_t val = A055938_b[next_index / 2] + (seq_t)next_index;
        A055938_grow_b(A055938_b_len + 1);
        A055938_b[A055938_b_len++] = val;
        A055938_ensure_present_size(val);
        A055938_present[(size_t)val] = true;
    }
}

static seq_t A055938_a(idx_t n)
{
    if (n == 0) return 0;
    while (A055938_missing_len < (size_t)n) {
        seq_t cur = (seq_t)A055938_next_cand;
        A055938_ensure_b_up_to_value(cur);
        if (!A055938_present[(size_t)cur]) {
            if (A055938_missing_len == A055938_missing_cap)
                A055938_grow_missing(A055938_missing_len + 1);
            A055938_missing[A055938_missing_len++] = cur;
        }
        ++A055938_next_cand;
    }
    return A055938_missing[(size_t)n - 1];
}

/* ---------- A005187 on a seq_t argument ---------- */
static seq_t A005187_a(seq_t n)
{
    seq_t result = 0;
    seq_t power = n;
    while (power > 0) {
        result += power;
        power >>= 1;
    }
    return result;
}

/* ---------- A064989 (result < n for odd n, fits in uint64_t) ---------- */
static uint64_t A064989_prev_prime(uint64_t n)
{
    uint64_t p = n - 1;
    for (;; --p) {
        if (is_prime((idx_t)p)) return p;
    }
}

static uint64_t A064989_a(uint64_t n)
{
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);
    uint64_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (pr[i] == 2) continue;
        uint64_t base = A064989_prev_prime(pr[i]);
        for (uint64_t k = 0; k < ex[i]; ++k) result *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    if ((n & 1) == 0) return A055938_a(n / 2);
    uint64_t m = A064989_a((uint64_t)n);
    return A005187_a(a((idx_t)m));
}
