/*
 * A289283  Starting with a(1) = 1, a(n) = smallest nonnegative integer not
 *          yet in the sequence such that the last digit of a(n-1) plus the
 *          first digit of a(n) is equal to 5. The digit 0 is not allowed.
 * https://oeis.org/A289283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A289283
 *  a(1) = 1.
 *  a(n) = smallest non‑negative integer not yet used,
 *  with no digit 0, such that
 *  first digit of a(n) + last digit of a(n‑1) = 5.
 *  The sequence stops when a term ends with a 5.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/get_digits.h"

static seq_t *seq      = NULL;   /* computed terms                        */
static idx_t seq_len   = 0;      /* number of terms currently stored      */
static idx_t seq_cap   = 0;      /* allocated capacity of seq[]           */

static uint8_t *used    = NULL;  /* used[x] == 1 iff x already appears    */
static idx_t used_cap   = 0;      /* allocated capacity of used[]          */

static bool finished = false;    /* true when a term ending in 5 is found */

/* enlarge seq[] so that it can hold at least 'need' elements      */
static void ensure_seq_cap(idx_t need)
{
    if (need <= seq_cap) return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* enlarge used[] so that it can index at least 'need' integers     */
static void ensure_used_cap(idx_t need)
{
    if (need <= used_cap) return;
    idx_t new_cap = used_cap ? used_cap * 2 : 32;
    while (new_cap < need) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(used, new_cap * sizeof(uint8_t));
    if (!tmp) abort();
    /* initialise the newly added part to zero */
    memset(tmp + used_cap, 0, (new_cap - used_cap));
    used = tmp;
    used_cap = new_cap;
}

/* generate the next term of the sequence                           */
static void build_next(void)
{
    if (finished) return;

    /* initialise first term */
    if (seq_len == 0) {
        ensure_seq_cap(1);
        seq[0] = (seq_t)1;
        seq_len = 1;
        ensure_used_cap(2);       /* we will refer to used[1] */
        used[1] = 1;
        return;
    }

    /* last digit of previous term (digits returned least‑significant first) */
    uint8_t buf_prev[64];
    size_t len_prev;
    get_digits(seq[seq_len - 1], buf_prev, &len_prev);
    uint8_t last_digit_prev = buf_prev[0];   /* least‑significant digit */

    /* search for smallest unused candidate that satisfies the rule */
    for (idx_t cand = 1; ; ++cand) {
        ensure_used_cap(cand + 1);          /* we refer to used[cand] */
        if (used[cand]) continue;

        /* digits of candidate */
        uint8_t buf_cand[64];
        size_t len_cand;
        get_digits((seq_t)cand, buf_cand, &len_cand);

        /* reject any zero digit */
        bool bad = false;
        for (size_t i = 0; i < len_cand; ++i) {
            if (buf_cand[i] == 0) { bad = true; break; }
        }
        if (bad) continue;

        uint8_t first_digit_cand = buf_cand[len_cand - 1];   /* most‑significant digit */
        if (first_digit_cand + last_digit_prev != 5) continue;

        /* acceptable candidate found */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len] = (seq_t)cand;
        seq_len++;
        used[cand] = 1;

        /* terminate if last digit of new term is 5 */
        if (buf_cand[0] == 5) finished = true;
        return;
    }
}

/* public entry point ---------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour below offset is irrelevant */
    while (seq_len < n) build_next();
    if (seq_len < n) return 0;     /* n beyond end of sequence */
    return seq[n - 1];
}
