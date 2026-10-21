/*
 * A285200  a(n) = floor the elevator is on at the n-th stage of Ken
 *          Knowlton's elevator problem, version 1.
 * https://oeis.org/A285200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9814, u16 n <= 9814, u32 n <= 9814, u64 n <= 9814,
 *            u128 n <= 9814, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A285200.
   The elevator starts at floor 1 and always steps up from floor 1.
   From each floor m it steps up every m‑th time it stops there,
   otherwise it goes down. This function simulates that process
   and returns a(n).  The sequence is 1‑indexed: a(1)=1. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Global state for memoisation. The arrays are grown lazily. */
static seq_t *seq = NULL;                      /* computed values a(1)…a(seq_len) */
static size_t seq_cap = 0;                     /* allocated size of seq */
static size_t seq_len = 0;                     /* number of values stored */

static idx_t *floor_counts = NULL;             /* visits of each floor */
static idx_t floor_cap = 0;                    /* allocated size of floor_counts */
static idx_t current_floor = 1;                /* current elevator position */

/* Ensure seq array can hold at least 'need' elements. */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    if (new_cap < need) new_cap = need;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure floor_counts array can index at least 'need'.  Newly
   allocated entries are zeroed. */
static void ensure_floor_cap(idx_t need)
{
    if (need <= floor_cap) return;
    size_t new_cap = floor_cap ? floor_cap * 2 : 128;
    while ((idx_t)new_cap < need) new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(floor_counts, new_cap * sizeof(idx_t));
    /* zero the newly allocated part */
    memset(tmp + floor_cap, 0, (new_cap - floor_cap) * sizeof(idx_t));
    floor_counts = tmp;
    floor_cap = new_cap;
}

/* Return a(n) for n >= 1.  For n == 0 the function returns 0
   (not part of the OEIS sequence). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety for out of range */

    /* Initialisation on first call. */
    if (seq == NULL) {
        ensure_seq_cap(128);
        seq[0] = (seq_t)1;                /* a(1) = 1 */
        seq_len = 1;
        ensure_floor_cap(128);
        /* First visit to floor 1 */
        floor_counts[1] = (idx_t)1;
        current_floor = 1;
    }

    /* Grow the seq array if required. */
    if ((size_t)n > seq_cap) ensure_seq_cap((size_t)n);

    /* Simulate until we have at least n values. */
    while (seq_len < (size_t)n) {
        idx_t visits = floor_counts[current_floor];
        bool step_up = (visits % current_floor == 0);
        idx_t next_floor = step_up ? current_floor + 1 : current_floor - 1;

        current_floor = next_floor;
        ensure_floor_cap(current_floor);
        floor_counts[current_floor]++;   /* count this arrival */

        /* Append the new floor to seq. */
        seq[seq_len++] = (seq_t)current_floor;
    }

    /* a(n) is stored at index n-1 (0‑based). */
    return seq[n - 1];
}
