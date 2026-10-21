/*
 * A299977  Lexicographic first sequence of positive integers such that a(n) +
 *          a(n+1) has a digit 7, and no term occurs twice.
 * https://oeis.org/A299977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: lexicographically first permutation of the positive integers
 * such that for every n >= 1   a(n)+a(n+1) contains the digit 7,
 * and no term occurs twice.  The sequence is built greedily:
 * at each step pick the smallest unused integer that satisfies the digit‑7
 * condition with the previous term.  Results are memoised, so calls
 * arrive in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* --- static state for memoisation ------------------------------------- */
static seq_t *seq      = NULL;  /* computed sequence values           */
static size_t seq_cap  = 0;     /* capacity of seq   (in terms)      */
static size_t seq_len  = 0;     /* number of stored terms             */

static unsigned char *used   = NULL;  /* bitmap of used numbers              */
static size_t        used_cap = 0;     /* capacity of used  bitmap (in bytes) */

/* Grow seq so it can hold at least n terms (1‑based). */
static void ensure_seq_cap(size_t n)
{
    if (n > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 8;
        while (new_cap < n) new_cap <<= 1;
        seq_t *p = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq = p;
        seq_cap = new_cap;
    }
}

/* Grow the used bitmap so it can contain index n. */
static void ensure_used_cap(size_t n)
{
    /* we need capacity > n, because index n is stored at used[n] */
    if (n >= used_cap) {
        size_t new_cap = used_cap ? used_cap << 1 : 8;
        while (new_cap <= n) new_cap <<= 1;
        unsigned char *p = (unsigned char *)realloc(used, new_cap);
        /* Zero the newly allocated part */
        memset(p + used_cap, 0, new_cap - used_cap);
        used = p;
        used_cap = new_cap;
    }
}

/* Return true iff decimal representation of x contains digit 7. */
static bool has_digit7(seq_t x)
{
    if (x == 0) return false;
    while (x > 0) {
        if ((x % 10) == 7) return true;
        x /= 10;
    }
    return false;
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety guard for 0‑based callers */

    /* initialise first term if not yet done */
    if (seq_len == 0) {
        ensure_seq_cap(1);
        seq[0] = (seq_t)1;
        seq_len = 1;
        ensure_used_cap(1);
        used[1] = 1;
    }

    /* extend the memoised part up to index n (1‑based) */
    while (seq_len < (size_t)n) {
        seq_t prev = seq[seq_len - 1];
        size_t cand = 1;
        for (;;) {
            ensure_used_cap(cand);
            if (used[cand]) {
                cand++;
                continue;
            }
            seq_t sum = prev + (seq_t)cand;
            if (has_digit7(sum))
                break;      /* candidate found */
            cand++;
        }

        /* store the new term */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len] = (seq_t)cand;
        seq_len++;
        used[cand] = 1;
    }

    return seq[n - 1];   /* convert 1‑based index to 0‑based */
}
