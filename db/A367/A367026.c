/*
 * A367026  a(1) = 0, a(2) = 1; thereafter a(n) is the smallest index < n not
 *          equal to i +- a(i) for any i = 1..n-1.
 * https://oeis.org/A367026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 498, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A367026: a(1)=0, a(2)=1; for n>2 a(n) is the smallest index m<n such that m ≠ i ± a(i) for all i<n. Computed by an incremental sieve. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t *a_vals = NULL;          /* sequence values, 1‑based */
static bool *banned = NULL;           /* banned indices, 1‑based */
static size_t a_cap = 0;              /* capacity of a_vals          */
static size_t banned_cap = 0;         /* capacity of banned          */
static idx_t computed_max = 0;        /* largest n already computed  */
static idx_t candidate = 1;           /* current smallest free index */

static void ensure_a(size_t need)
{
    if (need <= a_cap) return;
    size_t new_cap = a_cap ? a_cap << 1 : 4;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    a_vals = tmp;
    for (size_t i = a_cap; i < new_cap; ++i)
        a_vals[i] = (seq_t)0;
    a_cap = new_cap;
}

static void ensure_banned(size_t need)
{
    if (need <= banned_cap) return;
    size_t new_cap = banned_cap ? banned_cap << 1 : 8;
    while (new_cap < need) new_cap <<= 1;
    bool *tmp = (bool *)realloc(banned, new_cap * sizeof(bool));
    if (!tmp) exit(1);
    banned = tmp;
    for (size_t i = banned_cap; i < new_cap; ++i)
        banned[i] = false;
    banned_cap = new_cap;
}

static void init_state(void)
{
    if (computed_max) return;               /* already initialized */

    ensure_a(4);      /* need indices 0..3 at least */
    ensure_banned(4); /* banned indices up to 3 */

    a_vals[1] = (seq_t)0;
    a_vals[2] = (seq_t)1;
    computed_max = 2;

    /* mark positions banned by i = 1 (a(1)=0) */
    idx_t pos1 = 1 + (idx_t)a_vals[1];        /* 1 */
    idx_t pos2 = 1 - (idx_t)a_vals[1];        /* 1 */
    if (pos1 >= (idx_t)banned_cap) ensure_banned((size_t)pos1 + 1);
    banned[(size_t)pos1] = true;
    if (pos2 > 0) {
        if (pos2 >= (idx_t)banned_cap) ensure_banned((size_t)pos2 + 1);
        banned[(size_t)pos2] = true;
    }

    /* mark positions banned by i = 2 (a(2)=1) */
    pos1 = 2 + (idx_t)a_vals[2];              /* 3 */
    pos2 = 2 - (idx_t)a_vals[2];              /* 1 */
    if (pos1 >= (idx_t)banned_cap) ensure_banned((size_t)pos1 + 1);
    banned[(size_t)pos1] = true;
    if (pos2 > 0) {
        if (pos2 >= (idx_t)banned_cap) ensure_banned((size_t)pos2 + 1);
        banned[(size_t)pos2] = true;
    }

    candidate = 1;
}

static void compute_up_to(idx_t target)
{
    init_state();
    if (target <= computed_max) return;

    ensure_a((size_t)target + 1);

    for (idx_t i = computed_max + 1; i <= target; ++i) {
        /* find smallest candidate < i that is not banned yet */
        while (candidate <= i - 1 && banned[(size_t)candidate]) ++candidate;
        a_vals[i] = (seq_t)candidate;

        /* mark positions reachable from i */
        idx_t pos1 = i + (idx_t)a_vals[i];
        if (pos1 >= (idx_t)banned_cap) ensure_banned((size_t)pos1 + 1);
        banned[(size_t)pos1] = true;

        idx_t pos2 = i - (idx_t)a_vals[i];
        if (pos2 > 0) {
            if (pos2 >= (idx_t)banned_cap) ensure_banned((size_t)pos2 + 1);
            banned[(size_t)pos2] = true;
        }
    }
    computed_max = target;
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return a_vals[n];
}
