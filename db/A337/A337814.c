/*
 * A337814  a(n) is the smallest primitive nondeficient number that divides n
 *          or is a multiple of n.
 * https://oeis.org/A337814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A337814
 * a(n) = smallest primitive nondeficient number that divides n or is a multiple of n.
 * The implementation follows the definition directly.  For a given n:
 *   • if n itself is primitive nondeficient, a(n)=n;
 *   • otherwise, if n is nondeficient, a(n) is the smallest primitive
 *     divisor of n;
 *   • if n is deficient, a(n) is the smallest primitive multiple of n.
 *
 * The functions below use the existing utilities in ../utils/factor_u64.h
 * to factor integers.  All arithmetic is performed in seq_t to keep
 * arbitrary‑precision builds correct.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

/* Compute the sum of divisors σ(n).  The result is of type seq_t. */
static seq_t sigma_uint64(uint64_t n) {
    if (n <= 1) return (seq_t)n;
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);
    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = 1;
        seq_t powp  = 1;
        for (uint64_t k = 0; k < ex[i]; ++k) {
            powp *= pr[i];
            term += powp;
        }
        res *= term;
    }
    return res;
}

/* Return true iff n is nondeficient (σ(n) ≥ 2n). */
static bool is_nondef(uint64_t n) {
    if (n <= 1) return false;
    seq_t s   = sigma_uint64(n);
    seq_t two = (seq_t)2 * (seq_t)n;
    return s >= two;
}

/* Recursively search for a nondeficient proper divisor of m. */
static void dfs_has_nondef(uint64_t idx,
                           uint64_t cur,
                           const uint64_t *pr,
                           const uint64_t *ex,
                           size_t cnt,
                           uint64_t m,
                           bool *found) {
    if (*found) return;
    if (idx == cnt) {
        if (cur == m) return;                /* skip the whole number itself */
        if (is_nondef(cur)) *found = true;
        return;
    }
    uint64_t p  = pr[idx];
    uint64_t e  = ex[idx];
    uint64_t val = 1;
    for (uint64_t k = 0; k <= e; ++k) {
        dfs_has_nondef(idx + 1, cur * val, pr, ex, cnt, m, found);
        if (*found) return;
        val *= p;
    }
}

/* Return true iff m is a primitive nondeficient number. */
static bool is_primitive_uint64(uint64_t m) {
    if (!is_nondef(m)) return false;
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(m, pr, ex, &cnt);
    bool has = false;
    dfs_has_nondef(0, 1, pr, ex, cnt, m, &has);
    return !has;
}

/* Recursively find the smallest primitive divisor of n (excluding n itself). */
static void dfs_min_primitive(uint64_t idx,
                              uint64_t cur,
                              const uint64_t *pr,
                              const uint64_t *ex,
                              size_t cnt,
                              uint64_t n,
                              uint64_t *best) {
    if (idx == cnt) {
        if (cur == n) return;
        if (is_primitive_uint64(cur)) {
            if (*best == 0 || cur < *best) *best = cur;
        }
        return;
    }
    uint64_t p  = pr[idx];
    uint64_t e  = ex[idx];
    uint64_t val = 1;
    for (uint64_t k = 0; k <= e; ++k) {
        dfs_min_primitive(idx + 1, cur * val, pr, ex, cnt, n, best);
        val *= p;
    }
}

/* Return the smallest primitive divisor of n.  caller guarantees n is nondeficient
 * and not primitive itself. */
static uint64_t min_primitive_divisor(uint64_t n) {
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);
    uint64_t best = 0;
    dfs_min_primitive(0, 1, pr, ex, cnt, n, &best);
    return best;
}

/* Smallest primitive multiple of a deficient number n (n > 0). */
static seq_t smallest_primitive_multiple(uint64_t n) {
    for (uint64_t k = 1; ; ++k) {
        uint64_t m = n * k;
        if (is_primitive_uint64(m)) return (seq_t)m;
    }
}

/* Public sequence function. */
seq_t a(idx_t n_input) {
    if (n_input < 1) return 0;           /* sequence defined for n ≥ 1 only */

    uint64_t n = (uint64_t)n_input;

    if (is_primitive_uint64(n))
        return (seq_t)n;

    if (is_nondef(n))
        return (seq_t)min_primitive_divisor(n);

    return smallest_primitive_multiple(n);
}
