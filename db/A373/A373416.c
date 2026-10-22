/*
 * A373416  Fixed points of A323712: sizes k of card decks that return to
 *          original order after k pile shuffles (but not earlier).
 * https://oeis.org/A373416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer k such that the pile‑shuffle
 *            permutation of a deck of size k has exact order k. The values
 *            are computed by a search: we generate deck sizes in increasing
 *            order, compute the order of the corresponding permutation by
 *            cycle inspection, and keep those for which the ord...
 * Range:     u8 n <= 27, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Utility: greatest common divisor for 128‑bit integers */
static inline unsigned __int128 gcd128(unsigned __int128 a, unsigned __int128 b)
{
    while (b) {
        unsigned __int128 r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* ---------- Visited marker array for cycle inspection ---------- */
static uint64_t *vis_ts   = NULL;
static size_t   vis_cap   = 0;
static uint64_t vis_mark  = 1;

/* Ensure the marker array can hold at least n elements */
static void ensure_vis_cap(idx_t n)
{
    if (vis_cap < (size_t)n) {
        uint64_t *tmp = (uint64_t *)realloc(vis_ts, (size_t)n * sizeof(uint64_t));
        if (!tmp) exit(1);
        vis_ts = tmp;
        vis_cap = (size_t)n;
    }
}

/* Test whether a deck of size n returns to its original order after n
   pile shuffles (i.e., the permutation has order exactly n).          */
static bool is_fixed_point(idx_t n)
{
    if (n == 0) return false;

    ensure_vis_cap(n);

    /* Advance to a fresh visitation mark */
    vis_mark++;
    if (vis_mark == 0) {              /* overflow – reset array */
        memset(vis_ts, 0, vis_cap * sizeof(uint64_t));
        vis_mark = 1;
    }

    const idx_t odd = (n + 1) / 2;    /* number of odd positions */
    unsigned __int128 lcm = 1;

    for (idx_t start = 0; start < n; ++start) {
        if (vis_ts[start] == vis_mark) continue;

        /* Follow a new cycle originating at 'start' */
        idx_t cur = start;
        idx_t len = 0;
        while (vis_ts[cur] != vis_mark) {
            vis_ts[cur] = vis_mark;
            idx_t next;
            if ((cur & 1) == 0) {                /* even index -> odd card */
                next = odd - 1 - cur / 2;
            } else {                             /* odd index -> even card */
                next = n - 1 - (cur - 1) / 2;
            }
            cur = next;
            ++len;
        }

        /* Update lcm of cycle lengths */
        unsigned __int128 len128 = (unsigned __int128)len;
        unsigned __int128 g = gcd128(lcm, len128);
        unsigned __int128 t = lcm / g;
        if (t > (unsigned __int128)n / len128) return false;
        lcm = t * len128;
        if (lcm > (unsigned __int128)n) return false;
    }

    return lcm == (unsigned __int128)n;
}

/* ---------- Dynamic array of sequence values ---------- */
static seq_t  *fixed_vals = NULL;
static size_t  n_vals     = 0;
static size_t  cap_vals   = 0;
static idx_t   next_candidate = 1;   /* next deck size to test */

/* Ensure the result vector can hold at least m elements */
static void ensure_fixed_cap(size_t m)
{
    if (cap_vals < m) {
        size_t new_cap = cap_vals ? cap_vals * 2 : 16;
        while (new_cap < m) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(fixed_vals, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        fixed_vals = tmp;
        cap_vals   = new_cap;
    }
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined before offset */

    while (n_vals < (size_t)n) {
        while (!is_fixed_point(next_candidate)) ++next_candidate;
        ensure_fixed_cap(n_vals + 1);
        fixed_vals[n_vals++] = (seq_t)next_candidate;
        ++next_candidate;
    }
    return fixed_vals[n - 1];
}
