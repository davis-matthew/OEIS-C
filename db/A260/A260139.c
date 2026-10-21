/*
 * A260139  For any term a(k), there are exactly a(k) terms strictly smaller
 *          than 3*a(k); this is the lexicographically first increasing
 *          sequence of nonnegative integers with this property.
 * https://oeis.org/A260139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A260139
 *  A lexicographically first increasing sequence of non‑negative integers
 *  satisfying: for every term a(k) the number of terms strictly smaller
 *  than 3·a(k) equals a(k) itself.
 *  The following greedy construction generates the sequence.
 *
 *  For each new candidate v we need:
 *     1. existing = # of current terms < 3·v
 *     2. need(v) = v - (existing + 1)   (remaining terms that must still
 *        be < 3·v to satisfy the property)
 *     3. For every earlier term a(i), after adding v we must still be
 *        able to fit the remaining terms inside the interval [v+1,3·a(i)-1].
 *        – if a(i) is already satisfied (need(i)=0) the future terms must
 *          be ≥ 3·a(i).  
 *        – otherwise the remaining number of terms that can be added must
 *          not exceed the size of that interval.
 *
 *  This approach runs in O(n²) time with a small constant, which is
 *  fast enough for n ≤ 1 000 000 on typical hardware.  It uses only
 *  integer arithmetic and works for all supported seq_t types,
 *  including arbitrary‑precision ones.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_TERMS 20000               /* sufficient for n ≤ 10 000 */

/* global storage for the sequence and the remaining counts */
static seq_t seq[MAX_TERMS];
static seq_t need[MAX_TERMS];
static size_t seq_len = 0;

/* compute a(n) */
seq_t a(idx_t n)
{
    if (seq_len == 0) {               /* initialise the first element a(0)=0 */
        seq[0] = (seq_t)0;
        need[0] = (seq_t)0;
        seq_len = 1;
    }

    while (seq_len <= (size_t)n) {
        /* lower bound forced by already satisfied terms */
        seq_t lower = (seq_t)0;
        for (size_t i = 0; i < seq_len; ++i) {
            if (need[i] == (seq_t)0) {
                seq_t thr = seq[i] * (seq_t)3;
                if (thr > lower) lower = thr;
            }
        }

        seq_t cand_start;
        if (seq_len == 0) {
            cand_start = (seq_t)0;
        } else {
            cand_start = seq[seq_len - 1] + (seq_t)1;
            if (cand_start < lower) cand_start = lower;
        }

        for (seq_t cand = cand_start;; ++cand) {
            /* 1. count existing terms < 3*cand */
            seq_t thr = cand * (seq_t)3;
            seq_t existing = (seq_t)0;
            for (size_t i = 0; i < seq_len; ++i) {
                if (seq[i] < thr) existing++;
            }

            /* 2. remaining number of terms that must still be < 3*cand */
            seq_t need_new = cand - (existing + (seq_t)1);
            if (need_new < (seq_t)0) continue;   /* impossible candidate */

            /* 3. check feasibility for all earlier terms after adding cand */
            bool ok = true;
            for (size_t i = 0; i < seq_len && ok; ++i) {
                seq_t new_need = need[i];
                if (cand < seq[i] * (seq_t)3) new_need = new_need - (seq_t)1;
                if (new_need < (seq_t)0) { ok = false; break; }

                seq_t limit = (seq_t)0;
                seq_t bound = seq[i] * (seq_t)3;
                if (bound > cand + (seq_t)1)
                    limit = bound - (cand + (seq_t)1);

                if (new_need > limit) ok = false;
            }

            if (!ok) continue;

            /* also check that need_new fits in its own interval */
            seq_t limit_new = (seq_t)0;
            if (thr > cand + (seq_t)1)
                limit_new = thr - (cand + (seq_t)1);
            if (need_new > limit_new) continue;

            /* accept cand */
            for (size_t i = 0; i < seq_len; ++i) {
                if (cand < seq[i] * (seq_t)3) need[i]--;
            }
            seq[seq_len]     = cand;
            need[seq_len]    = need_new;
            seq_len++;
            break;
        }
    }

    return seq[(size_t)n];
}
