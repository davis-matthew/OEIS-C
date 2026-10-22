/*
 * A377225  a(0) = 0, a(1) = 1, a(2) = -1, a(3) = 2, and for n > 3, a(n) is
 *          the least integer (in absolute value) not yet in the sequence
 *          sharing a factor with a(n-1); in case of a tie, preference is
 *          given to the positive value.
 * https://oeis.org/A377225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 224, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A377225: a(0)=0, a(1)=1, a(2)=-1, a(3)=2, for n>3 the
   least unused integer sharing a factor with a(n‑1) (minimal
   absolute value; tie → positive). */

/* required external utilities */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */

static seq_t *seq = NULL;           /* values of the sequence */
static size_t seq_cap = 0, seq_len = 0;

static uint8_t *used_mask = NULL;   /* bit0 = positive used, bit1 = negative used */
static size_t used_cap = 0;

/* ensure used_mask has room for index v */
static void ensure_used_mask(size_t v)
{
    if (v < used_cap)
        return;
    size_t new_cap = used_cap ? used_cap : 16;
    while (new_cap <= v)
        new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(used_mask, new_cap * sizeof(uint8_t));
    if (tmp == NULL)
        abort();
    for (size_t i = used_cap; i < new_cap; ++i)
        tmp[i] = 0;
    used_mask = tmp;
    used_cap  = new_cap;
}

/* grow sequence up to target_len (inclusive) */
static void grow_seq(size_t target_len)
{
    if (seq_len > target_len)
        return;

    if (!seq || seq_cap < target_len + 1) {
        size_t new_cap = seq_cap ? seq_cap : 16;
        while (new_cap < target_len + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            abort();
        seq = tmp;
        seq_cap = new_cap;
    }

    for (size_t i = seq_len; i <= target_len; ++i) {
        seq_t val;
        if (i == 0)            val = (seq_t)0;
        else if (i == 1)       val = (seq_t)1;
        else if (i == 2)       val = (seq_t)-1;
        else if (i == 3)       val = (seq_t)2;
        else {
            seq_t prev = seq[i - 1];
            uint64_t abs_prev = prev >= 0 ? (uint64_t)prev : (uint64_t)(-prev);

            /* factor abs_prev */
            uint64_t primes[64];
            uint64_t exps[64];
            size_t prim_cnt = 0;
            if (abs_prev > 1)
                factor_u64(abs_prev, primes, exps, &prim_cnt);

            uint64_t best_abs = UINT64_MAX;
            int best_sign = 0;          /* +1 or -1 */

            for (size_t p_idx = 0; p_idx < prim_cnt; ++p_idx) {
                uint64_t p = primes[p_idx];
                uint64_t m = p;
                for (;;) {
                    ensure_used_mask(m);
                    uint8_t mask = used_mask[m];
                    if (!(mask & 1)) {          /* positive available */
                        if (m < best_abs || (m == best_abs && best_sign > 0)) {
                            best_abs  = m;
                            best_sign = 1;
                        }
                        break;
                    }
                    else if (!(mask & 2)) {      /* negative available */
                        if (m < best_abs || (m == best_abs && best_sign > 0)) {
                            best_abs  = m;
                            best_sign = -1;
                        }
                        break;
                    }
                    m += p;
                }
            }

            /* prim_cnt should never be 0 for n>3, but guard anyway */
            if (best_abs == UINT64_MAX)
                val = (seq_t)0;
            else
                val = best_sign * (seq_t)best_abs;
        }

        seq[i] = val;

        /* record usage of this absolute value */
        if (i > 0) {
            seq_t v = seq[i];
            uint64_t abs_v = v >= 0 ? (uint64_t)v : (uint64_t)(-v);
            ensure_used_mask(abs_v);
            if (v > 0)
                used_mask[abs_v] |= 1;
            else if (v < 0)
                used_mask[abs_v] |= 2;
        }
    }

    seq_len = target_len + 1;
}

/* public function required by the problem */
seq_t a(idx_t n)
{
    grow_seq((size_t)n);
    return seq[(size_t)n];
}
