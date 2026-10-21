/*
 * A294552  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) + b(n-2) + n, where a(0) = 1, a(1) = 2, b(0) = 3, and
 *          (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A294552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A294552: a(n) = a(n-1)+a(n-2)+b(n-1)+b(n-2)+n
   with a(0)=1, a(1)=2, b(0)=3, and (a(n)) and (b(n)) being the
   increasing complementary sequences.  The function returns a(n)
   for any non‑negative n.  This implementation lazily expands the
   sequences and memoises results.  It works with unsigned integer
   types up to any size, including arbitrary‑precision seq_t. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Bookkeeping arrays for the sequences and the used flag table. */
static seq_t *a_vals = NULL;      /* a[0..] */
static seq_t *b_vals = NULL;      /* b[0..] */
static size_t a_cap    = 0;
static size_t b_cap    = 0;
static size_t a_used   = 0;       /* number of a entries computed */

static bool *used      = NULL;    /* used[x] true iff x already in a or b */
static size_t used_cap = 0;        /* size of used bitmap */

static idx_t next_candidate = 4;   /* next integer to consider for b[n] */

/* Ensure allocation for the a array up to index 'idx' (inclusive). */
static void ensure_a_cap(size_t idx)
{
    if (idx < a_cap) return;
    size_t newcap = a_cap ? a_cap << 1 : 16;
    while (newcap <= idx) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_vals, newcap * sizeof(seq_t));
    if (!tmp) exit(1);
    a_vals = tmp;
    a_cap = newcap;
}

/* Ensure allocation for the b array up to index 'idx' (inclusive). */
static void ensure_b_cap(size_t idx)
{
    if (idx < b_cap) return;
    size_t newcap = b_cap ? b_cap << 1 : 16;
    while (newcap <= idx) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(b_vals, newcap * sizeof(seq_t));
    if (!tmp) exit(1);
    b_vals = tmp;
    b_cap = newcap;
}

/* Make sure the 'used' bitmap is large enough to index 'next_candidate'.
   Also mark all a values that now fit into the table. */
static void ensure_used_cap(void)
{
    while ((idx_t)next_candidate >= (idx_t)used_cap) {
        size_t newcap = used_cap ? used_cap << 1 : 64;
        if (newcap <= (size_t)next_candidate) newcap = next_candidate + 1024;
        bool *tmp = (bool *)realloc(used, newcap * sizeof(bool));
        if (!tmp) exit(1);
        memset(tmp + used_cap, 0, (newcap - used_cap) * sizeof(bool));
        used = tmp;
        /* Mark any a values that now fall inside the bitmap */
        for (size_t j = 0; j <= a_used; ++j) {
            seq_t val = a_vals[j];
            if (val <= (seq_t)newcap) {
                used[(size_t)val] = true;
            }
        }
        used_cap = newcap;
    }
}

/* Return the nth term of sequence a, expanding data as needed. */
seq_t a(idx_t n)
{
    /* Lazy initialisation */
    if (!a_vals) {
        a_cap = 16;
        a_vals = (seq_t *)calloc(a_cap, sizeof(seq_t));
        b_cap = 16;
        b_vals = (seq_t *)calloc(b_cap, sizeof(seq_t));
        used_cap = 64;
        used = (bool *)calloc(used_cap, sizeof(bool));
        if (!a_vals || !b_vals || !used) exit(1);

        /* initial terms */
        a_vals[0] = (seq_t)1;
        a_vals[1] = (seq_t)2;
        b_vals[0] = (seq_t)3;
        a_used = 1;

        used[1] = used[2] = used[3] = true;  /* mark 1,2,3 as used */
    }

    /* Expand the sequences up to the requested index. */
    if (n > a_used) {
        for (idx_t i = a_used + 1; i <= n; ++i) {
            /* Determine b(i-1) as the smallest unused integer */
            ensure_used_cap();
            while (used[(size_t)next_candidate]) ++next_candidate;
            seq_t b_val = (seq_t)next_candidate;
            if (b_val <= (seq_t)used_cap) used[(size_t)b_val] = true;
            ensure_b_cap((size_t)i - 1);
            b_vals[(size_t)i - 1] = b_val;

            /* Compute a(i) using the recurrence */
            seq_t a_val =
                a_vals[(size_t)i - 1] + a_vals[(size_t)i - 2] +
                b_vals[(size_t)i - 1] + b_vals[(size_t)i - 2] + (seq_t)i;
            ensure_a_cap((size_t)i);
            a_vals[(size_t)i] = a_val;
            if (a_val <= (seq_t)used_cap) used[(size_t)a_val] = true;

            a_used = i;
        }
    }
    return a_vals[(size_t)n];
}
