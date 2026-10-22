/*
 * A302717  Start with a(0) = 0, then append the terms in [x, 2*x+1, x*(x+1)]
 *          which do not occur earlier, for x = 1, 2, ...
 * https://oeis.org/A302717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) defined by the recurrence: a(0) = 0; for x = 1,2,… we
 *            append the numbers [x, 2*x+1, x*(x+1)] that have not appeared
 *            earlier. This yields a permutation of the non‑negative integers
 * Range:     u8 n <= 37, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Back‑end storage – static variables are initialised on first call. */
static seq_t   *g_seq   = NULL;
static size_t  g_seq_len = 0;
static size_t  g_seq_cap = 0;

static seq_t   *g_seen  = NULL;
static size_t  g_seen_len = 0;
static size_t  g_seen_cap = 0;

static idx_t   g_last_x = 0;
static bool    g_inited = false;

/* Ensure that seq has the requested capacity. */
static void
ensure_seq_cap(size_t new_cap)
{
    if (new_cap <= g_seq_cap)
        return;
    size_t cap = g_seq_cap ? g_seq_cap : 16;
    while (cap < new_cap)
        cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(g_seq, cap * sizeof(seq_t));
    if (!tmp) abort();                 /* out of memory */
    g_seq  = tmp;
    g_seq_cap = cap;
}

/* Ensure that seen has the requested capacity. */
static void
ensure_seen_cap(size_t new_cap)
{
    if (new_cap <= g_seen_cap)
        return;
    size_t cap = g_seen_cap ? g_seen_cap : 16;
    while (cap < new_cap)
        cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(g_seen, cap * sizeof(seq_t));
    if (!tmp) abort();
    g_seen  = tmp;
    g_seen_cap = cap;
}

/* Linear search in the seen array. */
static bool
seen_contains(seq_t val)
{
    for (size_t i = 0; i < g_seen_len; ++i)
        if (g_seen[i] == val)
            return true;
    return false;
}

/* Append a value to the seen array. */
static void
append_seen(seq_t val)
{
    ensure_seen_cap(g_seen_len + 1);
    g_seen[g_seen_len++] = val;
}

/* Append a value to the sequence array. */
static void
append_seq(seq_t val)
{
    ensure_seq_cap(g_seq_len + 1);
    g_seq[g_seq_len++] = val;
}

/* Initialise the data structures once. */
static void
init_once(void)
{
    if (g_inited)
        return;
    g_inited = true;
    g_last_x = 0;

    ensure_seq_cap(1);
    ensure_seen_cap(1);

    g_seq[0]   = (seq_t)0;
    g_seq_len  = 1;

    g_seen[0]  = (seq_t)0;
    g_seen_len = 1;
}

/* Main function required by the specification. */
seq_t a(idx_t n)
{
    init_once();

    /* Generate terms lazily up to index n. */
    while (g_seq_len <= (size_t)n) {
        ++g_last_x;
        seq_t xv = (seq_t)g_last_x;

        /* Candidate 1: x */
        if (!seen_contains(xv)) {
            append_seq(xv);
            append_seen(xv);
        }

        /* Candidate 2: 2*x + 1 */
        seq_t cand2 = xv * 2 + 1;
        if (!seen_contains(cand2)) {
            append_seq(cand2);
            append_seen(cand2);
        }

        /* Candidate 3: x*(x+1) */
        seq_t cand3 = xv * (xv + 1);
        if (!seen_contains(cand3)) {
            append_seq(cand3);
            append_seen(cand3);
        }
    }

    return g_seq[n];
}
