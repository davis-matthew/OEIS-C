/*
 * A299541  Solution a( ) of the complementary equation a(n) = b(n-1) +
 *          b(n-3), where a(0) = 2, a(1) = 4, a(2) = 6; see Comments.
 * https://oeis.org/A299541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=2, a(1)=4, a(2)=6;
   a(n) = b(n-1) + b(n-3)   for n > 3;
   b(0) = least positive integer NOT in {a(0),a(1),a(2)};
   b(n) = least positive integer NOT in {a(0),...,a(n),b(0),...,b(n-1)}   for n > 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n) {
    /* static state for memoisation */
    static bool   initialized = false;
    static idx_t  cached_n    = 0;
    static seq_t *a_arr       = NULL;
    static seq_t *b_arr       = NULL;
    static size_t a_cap       = 0, b_cap = 0;
    static uint8_t *used      = NULL;
    static size_t used_cap    = 0;
    static idx_t  next_candidate = 1;   /* smallest positive integer not yet used */

    /* one‑time initialisation */
    if (!initialized) {
        /* arrays for a() and b() up to index 2 */
        a_cap = 3;
        b_cap = 3;
        a_arr = (seq_t*)malloc(a_cap * sizeof(seq_t));
        b_arr = (seq_t*)malloc(b_cap * sizeof(seq_t));

        /* initial terms of a() */
        a_arr[0] = (seq_t)2;
        a_arr[1] = (seq_t)4;
        a_arr[2] = (seq_t)6;

        /* used[] – bitset of numbers that already appear */
        used_cap = 128;                      /* generous start */
        used     = (uint8_t*)calloc(used_cap, sizeof(uint8_t));   /* zero‑initialised */

        /* mark the numbers used so far: 1(b0),2,4,6 */
        used[1] = 1;
        used[2] = 1;
        used[4] = 1;
        used[6] = 1;

        /* b(0) = 1 */
        b_arr[0] = (seq_t)1;

        /* compute b(1) and b(2) */
        for (idx_t i = 1; i <= 2; ++i) {
            /* find next unused integer */
            while (1) {
                if ((size_t)next_candidate >= used_cap) {
                    size_t old = used_cap;
                    used_cap = (size_t)(next_candidate + 1);
                    used = (uint8_t*)realloc(used, used_cap);
                    memset(used + old, 0, used_cap - old);
                }
                if (!used[next_candidate]) break;
                ++next_candidate;
            }
            used[next_candidate] = 1;
            b_arr[i] = (seq_t)next_candidate;
            ++next_candidate;
        }

        cached_n    = 2;      /* a[0]..a[2] are known */
        initialized = true;
    }

    /* extend tables if required */
    if (n > cached_n) {
        /* grow a() and b() arrays */
        if (a_cap <= (size_t)n) {
            size_t new_cap = 2 * (n + 1);
            a_arr = (seq_t*)realloc(a_arr, new_cap * sizeof(seq_t));
            a_cap = new_cap;
        }
        if (b_cap <= (size_t)n) {
            size_t new_cap = 2 * (n + 1);
            b_arr = (seq_t*)realloc(b_arr, new_cap * sizeof(seq_t));
            b_cap = new_cap;
        }

        /* compute values for indices cached_n+1 .. n */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            if (i <= 2) continue;           /* base values already set */

            /* a(i) = b(i-1) + b(i-3) */
            seq_t val = b_arr[i-1] + b_arr[i-3];
            a_arr[i] = val;

            /* mark a(i) as used */
            size_t uidx = (size_t)val;
            if (uidx >= used_cap) {
                size_t old = used_cap;
                used_cap = uidx + 1;
                used = (uint8_t*)realloc(used, used_cap);
                memset(used + old, 0, used_cap - old);
            }
            used[uidx] = 1;

            /* compute b(i) – least unused integer */
            idx_t cand = next_candidate;
            while (1) {
                if ((size_t)cand >= used_cap) {
                    size_t old = used_cap;
                    used_cap = (size_t)cand + 1;
                    used = (uint8_t*)realloc(used, used_cap);
                    memset(used + old, 0, used_cap - old);
                }
                if (!used[cand]) break;
                ++cand;
            }
            used[cand] = 1;
            b_arr[i] = (seq_t)cand;
            next_candidate = cand + 1;
        }

        cached_n = n;
    }

    return a_arr[n];
}
