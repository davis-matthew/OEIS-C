/*
 * A327988  The zeros of A327987.
 * https://oeis.org/A327988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A327988: zeros of A327987 – computed by search.
   A(k) = 0 if for all divisors d of k, d & (k/d) == 0.
   Sequence starts with a(1)=0.  */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* helper: recursive divisor enumeration that tests the condition */
static bool check_divisors_rec(size_t idx, uint64_t cur, uint64_t k,
                               const uint64_t *pr, const uint64_t *ex, size_t cnt)
{
    if (idx == cnt) {
        uint64_t e = k / cur;
        return ((cur & e) == 0);
    }
    uint64_t val = cur;
    for (size_t e = 0; e <= ex[idx]; ++e) {
        if (!check_divisors_rec(idx + 1, val, k, pr, ex, cnt))
            return false;
        if (e < ex[idx])
            val *= pr[idx];
    }
    return true;
}

/* test whether k satisfies the all‑divisors AND‑zero condition (k>0) */
static bool k_is_good(uint64_t k)
{
    if (k == 0)          /* not used, handled separately          */
        return true;
    if (k == 1)          /* 1 has divisor 1: 1&1 != 0             */
        return false;

    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(k, pr, ex, &cnt);
    return check_divisors_rec(0, 1, k, pr, ex, cnt);
}

/* cache for the sequence */
static seq_t *seq_cache = NULL;
static size_t seq_cap   = 0;
static size_t seq_len   = 0;
static uint64_t next_k  = 1;  /* next candidate after 0 */

/* main sequence function */
seq_t a(idx_t n)
{
    /* 1st term a(1)=0 is stored explicitly */
    if (n == 1)
        return (seq_t)0;

    /* initialise cache on first call */
    if (seq_len == 0) {
        seq_cap = 16;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_cache[0] = (seq_t)0;          /* a(1) = 0 */
        seq_len = 1;
        next_k = 1;                       /* first positive to test */
    }

    /* extend cache until we have at least n terms */
    while (seq_len < (size_t)n) {
        uint64_t cand = (uint64_t)next_k;
        if (k_is_good(cand)) {
            if (seq_len >= seq_cap) {
                seq_cap *= 2;
                seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)cand;
        }
        ++next_k;
    }

    return seq_cache[n - 1];
}
