/*
 * A365219  Each term is a "Go up integer" (GUI), but a(n) + a(n+1) is always
 *          a "Go down integer" (GDI). More details in the Comments section.
 * https://oeis.org/A365219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A365219: each term is the smallest unused Go‑Up Integer
   (GUI) that, when added to the previous term, yields a Go‑Down
   Integer (GDI). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return the most significant (leftmost) decimal digit of x.  */
static inline seq_t digit_first(seq_t x)
{
    while (x >= (seq_t)10)
        x /= (seq_t)10;
    return x;
}

/* Return the least significant (rightmost) decimal digit of x.  */
static inline seq_t digit_last(seq_t x)
{
    return x % (seq_t)10;
}

/* GDI: leftmost digit > rightmost digit.  */
static inline bool is_gdi(seq_t x)
{
    return digit_first(x) > digit_last(x);
}

/* GUI: rightmost digit > leftmost digit.  */
static inline bool is_gui(seq_t x)
{
    return digit_first(x) < digit_last(x);
}

/* ------------------------------------------------------------------ */
/*  Memoised sequence data                                            */
/* ------------------------------------------------------------------ */
static seq_t *seq      = NULL;   /* sequence values */
static size_t  seq_len = 0;      /* number of stored terms */
static size_t  seq_cap = 0;      /* allocated capacity for seq */

static seq_t *gui_list = NULL;   /* all GUI values discovered so far */
static bool  *used     = NULL;   /* used[i] == true iff gui_list[i] already in seq */
static size_t  gui_len = 0;      /* number of GUI values stored */
static size_t  gui_cap = 0;      /* allocated capacity for gui_list */

static seq_t next_candidate = 13; /* next integer to test for being a GUI */

/* Ensure there is room for at least 'need' terms in the sequence array. */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t newcap = (seq_cap == 0 ? 16 : seq_cap * 2);
    while (newcap < need)
        newcap *= 2;
    seq = mem_resize(seq_t, seq, newcap);
    seq_cap = newcap;
}

/* Ensure there is room for at least 'need' GUI values in the gui array. */
static void ensure_gui_cap(size_t need)
{
    if (need <= gui_cap)
        return;
    size_t newcap = (gui_cap == 0 ? 16 : gui_cap * 2);
    while (newcap < need)
        newcap *= 2;
    gui_list = mem_resize(seq_t, gui_list, newcap);
    used     = (bool   *)realloc(used,     newcap * sizeof(bool));
    gui_cap  = newcap;
}

/* The required function.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for indices < 1 */

    /* Initialise on first call */
    if (seq_len == 0) {
        seq_cap = 16;
        seq = mem_new(seq_t, seq_cap);

        gui_cap = 16;
        gui_list = mem_new(seq_t, gui_cap);
        used     = (bool   *)calloc(gui_cap, sizeof(bool));

        /* First term: 12 is the smallest GUI. */
        seq[0]   = (seq_t)12;
        seq_len  = 1;

        gui_list[0] = (seq_t)12;
        used[0]     = true;
        gui_len     = 1;
    }

    /* Generate terms until we have at least n terms. */
    while (seq_len < n) {
        seq_t prev = seq[seq_len - 1];
        bool found = false;

        /* First try all GUIs discovered so far that are still unused. */
        for (size_t i = 0; i < gui_len && !found; ++i) {
            if (used[i])
                continue;
            seq_t cand = gui_list[i];
            if (is_gdi(prev + cand)) {
                ensure_seq_cap(seq_len + 1);
                seq[seq_len++] = cand;
                used[i] = true;
                found = true;
            }
        }

        /* If nothing found, generate further GUIs one by one until a match. */
        while (!found) {
            /* Find the next GUI (skipping non‑GUI integers). */
            while (!is_gui(next_candidate))
                ++next_candidate;

            /* Record this GUI as a discovered value. */
            ensure_gui_cap(gui_len + 1);
            gui_list[gui_len] = next_candidate;
            used[gui_len] = false;   /* not yet used in the sequence */
            size_t cur_idx = gui_len;
            ++gui_len;

            /* Test it with the previous term. */
            if (is_gdi(prev + next_candidate)) {
                ensure_seq_cap(seq_len + 1);
                seq[seq_len++] = next_candidate;
                used[cur_idx] = true;
                found = true;
            }

            /* Move on for the next search if needed. */
            ++next_candidate;
        }
    }

    return seq[n - 1];
}
