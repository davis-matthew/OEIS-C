/*
 * A344548  Lexicographically earliest sequence of nonnegative terms forming a
 *          clockwise square spiral when nothing else is read except the
 *          parity of the digits (see the Comments section).
 * https://oeis.org/A344548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest sequence of nonnegative terms forming a
   clockwise square spiral when nothing else is read except the parity
   of the digits (OEIS A344548).  The parity pattern is defined by
   the turn decisions needed for a clockwise square spiral: the first
   step has parity 0; every subsequent side of the spiral starts with a
   parity 1, followed by zeros for the remaining steps of that side. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Persistent state -------------------------------------------------------- */
static seq_t *seq_cache = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

static seq_t *used_vals = NULL;
static size_t used_len  = 0;
static size_t used_cap  = 0;

static unsigned char *parity_tab = NULL;
static size_t parity_len = 0;
static size_t parity_cap = 0;

static idx_t side_no       = 1;   /* current side number (1‑based)          */
static idx_t side_len      = 1;   /* length of the current side             */
static idx_t side_remaining = 1;   /* steps still left in the current side   */

static idx_t next_step = 1;        /* index of the next step (1‑based)       */

/* Helper prototypes ------------------------------------------------------- */
static unsigned char get_parity(idx_t pos);
static void ensure_cache_capacity(size_t needed);
static void ensure_used_capacity (size_t needed);
static bool is_used(seq_t val);
static void add_used(seq_t val);
static bool cand_matches_parity(seq_t cand, idx_t start_step, size_t *len);

/* ------------------------------------------------------------------------ */
static unsigned char get_parity(idx_t pos)
{
    /* Grow the parity table up to position `pos` (1‑based). */
    while (parity_len < (size_t)pos) {
        if (side_remaining == 0) {
            side_no++;
            side_len = (side_no + 1) / 2;   /* ceil(side_no/2)               */
            side_remaining = side_len;
        }

        unsigned char parity;
        if (side_remaining == side_len) {
            /* First step of a side: 0 for the very first side, 1 otherwise. */
            parity = (side_no == 1) ? 0 : 1;
        } else {
            parity = 0;
        }

        if (parity_len == parity_cap) {
            parity_cap = parity_cap ? parity_cap * 2 : 64;
            parity_tab = (unsigned char *)realloc(parity_tab, parity_cap);
        }
        parity_tab[parity_len++] = parity;
        side_remaining--;
    }
    return parity_tab[pos - 1];
}

/* ------------------------------------------------------------------------ */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_len >= needed) return;
    if (cache_cap < needed) {
        cache_cap = cache_cap ? cache_cap * 2 : 64;
        seq_cache = (seq_t *)realloc(seq_cache, cache_cap * sizeof(seq_t));
    }
}

/* ------------------------------------------------------------------------ */
static void ensure_used_capacity(size_t needed)
{
    if (used_len >= needed) return;
    if (used_cap < needed) {
        used_cap = used_cap ? used_cap * 2 : 64;
        used_vals = (seq_t *)realloc(used_vals, used_cap * sizeof(seq_t));
    }
}

/* ------------------------------------------------------------------------ */
static bool is_used(seq_t val)
{
    for (size_t i = 0; i < used_len; ++i)
        if (used_vals[i] == val) return true;
    return false;
}

/* ------------------------------------------------------------------------ */
static void add_used(seq_t val)
{
    ensure_used_capacity(used_len + 1);
    used_vals[used_len++] = val;
}

/* ------------------------------------------------------------------------ */
static bool cand_matches_parity(seq_t cand, idx_t start_step, size_t *len)
{
    unsigned char digs[64];
    size_t dlen = 0;
    seq_t tmp = cand;

    if (tmp == 0) {
        digs[0] = 0;
        dlen = 1;
    } else {
        while (tmp != 0) {
            digs[dlen++] = (unsigned char)(tmp % 10);
            tmp /= 10;
        }
    }

    for (size_t j = 0; j < dlen; ++j) {
        unsigned char digit = digs[dlen - 1 - j];        /* most to least   */
        idx_t       parity_idx = start_step + (idx_t)j;  /* 1‑based step    */
        unsigned char expected = get_parity(parity_idx);
        if ((digit & 1) != expected) return false;
    }
    *len = dlen;
    return true;
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Initialize static state on first call. */
    if (cache_len == 0) {
        cache_cap = 64;
        seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));

        used_cap = 64;
        used_vals = (seq_t *)malloc(used_cap * sizeof(seq_t));

        parity_cap = 64;
        parity_tab = (unsigned char *)malloc(parity_cap * sizeof(unsigned char));

        side_no = 1;
        side_len = 1;
        side_remaining = 1;
        next_step = 1;
    }

    if (n <= 0) return 0;          /* safety for non‑positive indices */

    /* Return cached value if present. */
    if (n <= cache_len) return seq_cache[n - 1];

    /* Build terms up to n. */
    for (size_t cur = cache_len + 1; cur <= (size_t)n; ++cur) {
        size_t dlen;
        seq_t val = 0;

        /* Find the smallest unused integer that matches the parity pattern. */
        for (seq_t cand = 0; ; ++cand) {
            if (is_used(cand)) continue;
            if (cand_matches_parity(cand, next_step, &dlen)) {
                val = cand;
                break;
            }
        }

        ensure_cache_capacity(cur);
        seq_cache[cache_len++] = val;
        add_used(val);

        next_step += (idx_t)dlen;
    }

    return seq_cache[n - 1];
}
