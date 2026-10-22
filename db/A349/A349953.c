/*
 * A349953  a(1)=0 and thereafter a(n) = Min a(i) over those i=1..n-2 with i +
 *          a(i+1) = n; or if no such i then a(n) is the least integer >= 2
 *          not in a(1)..a(n-1).
 * https://oeis.org/A349953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n>1, a(n)=min{a(i)} over i=1..n-2 such that i +
 *            a(i+1)=n; if no such i, a(n) is the least integer ≥2 not
 *            occurring in a(1)..a(n-1)
 * Range:     u8 n <= 566, u16 n <= 6000, u32 n <= 6000, u64 n <= 6000,
 *            u128 n <= 6000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies: seq_t, idx_t are provided by the build system. */

#include <stdlib.h>
#include <stdbool.h>

/* --- static data ---------------------------------------------- */
static seq_t *a_arr   = NULL;   /* sequence values (1‑based)          */
static seq_t *cand_arr= NULL;   /* minimal candidate for each index   */
static bool   *used_arr= NULL;   /* marks values that have already appeared */
static size_t  a_cap   = 0;      /* capacity of a_arr and cand_arr      */
static size_t  used_cap= 0;      /* capacity of used_arr                */
static idx_t   max_n   = 0;      /* highest n computed so far           */
static idx_t   next_filler = 2;  /* next integer ≥2 to try for filler   */
static const seq_t sentinel = (seq_t)(-1);

/* --- helper functions ---------------------------------------- */
static void ensure_capacity(idx_t n)
{
    if (a_cap >= (size_t)n + 2)   /* we need indices 0…n inclusive   */
        return;

    size_t new_cap = a_cap ? a_cap * 2 : 16;
    while (new_cap <= (size_t)n + 1)
        new_cap <<= 1;

    seq_t *new_a = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    seq_t *new_c = (seq_t *)realloc(cand_arr, new_cap * sizeof(seq_t));
    if (!new_a || !new_c)
        abort();

    for (size_t i = a_cap; i < new_cap; ++i) {
        new_a[i]   = 0;
        new_c[i]   = sentinel;
    }

    a_arr   = new_a;
    cand_arr= new_c;
    a_cap   = new_cap;
}

static void ensure_used_capacity(idx_t v)
{
    if (used_cap > (size_t)v)
        return;

    size_t new_used = used_cap ? used_cap * 2 : 16;
    while (new_used <= (size_t)v)
        new_used <<= 1;

    bool *new_used_arr = (bool *)realloc(used_arr, new_used * sizeof(bool));
    if (!new_used_arr)
        abort();

    for (size_t i = used_cap; i < new_used; ++i)
        new_used_arr[i] = false;

    used_arr = new_used_arr;
    used_cap = new_used;
}

static void init_first(void)
{
    if (max_n != 0)
        return;

    ensure_capacity(1);
    a_arr[1] = 0;

    ensure_used_capacity(0);
    used_arr[0] = true;  /* value 0 is present */

    next_filler = 2;
    max_n = 1;
}

/* --- public sequence function -------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n<1, but return 0 as a safe default */

    init_first();
    ensure_capacity(n);

    /* compute any missing terms up to n */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t val;

        /* check whether a candidate has already been scheduled */
        if (cand_arr[k] == sentinel) {
            /* no candidate: choose the smallest unused integer ≥2 */
            idx_t f = next_filler;
            for (;;) {
                ensure_used_capacity(f);
                if (!used_arr[f])
                    break;
                ++f;
            }
            val = (seq_t)f;
            next_filler = f + 1;
        } else {
            val = cand_arr[k];
        }

        a_arr[k] = val;

        /* schedule candidate for future positions using a(k‑1) */
        if (k >= 2) {
            idx_t target = (k - 1) + (idx_t)val;  /* i + a(i+1) */
            seq_t cand_val = a_arr[k - 1];        /* a(i) */

            if (target < a_cap) {
                if (cand_arr[target] == sentinel || cand_val < cand_arr[target])
                    cand_arr[target] = cand_val;
            } else {
                ensure_capacity(target);
                cand_arr[target] = cand_val;
            }
        }

        /* record value in used set for future filler decisions */
        idx_t v = (idx_t)val;           /* safe: val fits in idx_t for required n */
        ensure_used_capacity(v);
        used_arr[v] = true;
    }

    max_n = n;
    return a_arr[n];
}
