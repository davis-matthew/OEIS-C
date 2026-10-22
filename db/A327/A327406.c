/*
 * A327406  Number of steps to reach a fixed point starting with n and
 *          repeatedly taking the quotient by the maximum divisor that is 1 or
 *          whose prime indices have a common divisor > 1 (A327405, A327656).
 * https://oeis.org/A327406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps to reach 1 by repeatedly dividing n by
 *            the largest divisor d (>1) such that the prime indices of d have
 *            GCD > 1. If a number has no such divisor (i.e., a power of 2) it
 *            is a fixed point and contributes 0 steps
 * Range:     u8 n <= 9318, u16 n <= 9318, u32 n <= 9318, u64 n <= 9318,
 *            u128 n <= 9318, big any n
 * Helpers:   build_primes_upto, factor_u64, gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"           /* void factor_u64(uint64_t n,
                                               uint64_t *pr, uint64_t *ex,
                                               size_t *cnt); */
#include "../utils/build_primes_upto.h"    /* void build_primes_upto(idx_t limit,
                                               uint64_t **primes_out,
                                               size_t *cnt_out); */
#include "../utils/gcd_int.h"             /* int gcd_int(int a, int b); */

static bool primes_started = false;
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;

/* Initialise a list of primes once, up to a sufficiently large limit. */
static void init_primes(void) {
    if (!primes_started) {
        build_primes_upto(200000, &prime_list, &prime_cnt); /* enough for all n <= 1e5 */
        primes_started = true;
    }
}

/* Return the 1‑based index of prime p in the prime list. */
static int prime_index(uint64_t p) {
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        uint64_t q = prime_list[mid];
        if (q == p) return (int)(mid + 1);   /* indices start at 1 */
        if (q < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return -1;  /* should not happen for valid inputs */
}

/* Recursive enumeration of all divisors of n, keeping track of the GCD
   of prime indices present in the current divisor. */
static void dfs_div(uint64_t cur_val, size_t depth, int cur_gcd,
                    const uint64_t *pf, const uint64_t *ex,
                    const int *idx, size_t cnt, uint64_t *max_d) {
    if (depth == cnt) {
        if (cur_gcd > 1 && cur_val > *max_d)
            *max_d = cur_val;
        return;
    }

    /* Branch where this prime is absent (exponent 0). */
    dfs_div(cur_val, depth + 1, cur_gcd, pf, ex, idx, cnt, max_d);

    /* Branches where this prime appears with exponent >=1. */
    uint64_t val = cur_val;
    int g = cur_gcd;
    for (uint64_t e = 1; e <= ex[depth]; ++e) {
        val *= pf[depth];
        int new_gcd = (g == 0) ? idx[depth] : gcd_int(g, idx[depth]);
        if (new_gcd == 1)   /* further exponents cannot improve GCD */
            break;
        dfs_div(val, depth + 1, new_gcd, pf, ex, idx, cnt, max_d);
    }
}

/* Return the maximum divisor d>1 of n whose prime indices have GCD>1.
   If no such divisor exists, return 1. */
static uint64_t max_div_with_common_idx(uint64_t n) {
    if (n <= 1) return 1;

    init_primes();

    uint64_t prime_f[32], exps_f[32];
    size_t cnt_f = 0;
    factor_u64(n, prime_f, exps_f, &cnt_f);

    /* Compute the prime indices for each prime factor. */
    int idxs[32];
    for (size_t i = 0; i < cnt_f; ++i) {
        idxs[i] = prime_index(prime_f[i]);  /* should exist */
    }

    uint64_t max_d = 1;
    dfs_div(1, 0, 0, prime_f, exps_f, idxs, cnt_f, &max_d);
    return max_d;
}

/* ------------------------------------------------------------------- */
/* Public sequence function ------------------------------------------------ */

seq_t a(idx_t n) {
    /* Count of steps to reach 1. */
    seq_t steps = 0;

    uint64_t cur = (uint64_t)n;
    if (cur <= 1) return steps;

    /* Fast check: power of two => fixed point, 0 steps. */
    if ((cur & (cur - 1)) == 0)
        return steps;

    while (cur > 1 && (cur & (cur - 1)) != 0) {
        uint64_t d = max_div_with_common_idx(cur);
        /* d should be >1 for all non‑powers of two. */
        if (d <= 1) break;          /* safety check */
        cur /= d;
        ++steps;
    }

    return steps;
}
