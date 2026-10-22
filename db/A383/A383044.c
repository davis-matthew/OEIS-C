/*
 * A383044  Numbers m such that phi(m) + phi(m+phi(m)) = m where phi is the
 *          Euler totient function.
 * https://oeis.org/A383044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 74, u32 n <= 123, u64 n <= 123,
 *            u128 n <= 123, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A383044 : numbers m such that φ(m)+φ(m+φ(m)) = m.
   The terms are the increasing union of the six progressions:
       2^k              (k ≥ 2)
       2^k·3            (k ≥ 1)
       2^k·5            (k ≥ 1)
       2^k·7            (k ≥ 1)
       2^k·35           (k ≥ 1)
       2^k·47           (k ≥ 1)
   The function keeps the current value of each progression, repeatedly
   emits the smallest one, and memoises the results so that calls with
   arbitrary (non‑monotonic) n are fast.  Overflow of under‑lying
   unsigned types is detected by a wrap‑around test; when a progression
   overflows it is marked exhausted and no longer contributes
   further terms.  This keeps the sequence correct for all
   seq_t values that actually fit. */
#include <stddef.h>   /* size_t                */
#include <stdlib.h>   /* realloc */
#include <stdbool.h>  /* bool, true, false */
#include <stdint.h>   /* for uint8_t etc */

/* memoised sequence buffer */
static seq_t *g_seq = NULL;
static size_t g_len = 0;
static size_t g_cap = 0;

/* append a value to the memoised buffer, growing if necessary */
static void append_value(seq_t v)
{
    if (g_len == g_cap) {
        size_t newcap = g_cap ? g_cap * 2 : 128;
        seq_t *tmp = (seq_t *)realloc(g_seq, newcap * sizeof(seq_t));
        if (!tmp) return;                 /* silently abort on memory failure */
        g_seq = tmp;
        g_cap = newcap;
    }
    g_seq[g_len++] = v;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined for n < 1 */

    /* if already cached, return the value immediately */
    if (g_len >= n) return g_seq[n - 1];

    /* constants for the six progressions */
    const unsigned int factors[6] = {1, 3, 5, 7, 35, 47};
    const idx_t       kstart[6]   = {2, 1, 1, 1, 1, 1};

    /* current state of each progression */
    struct node_t {
        seq_t val;          /* current value of the progression */
        idx_t exp;          /* current exponent of 2 */
        unsigned int fact;  /* base factor */
        bool exhausted;     /* true if the progression has overflowed */
    } nodes[6];

    /* initialise the nodes */
    for (int i = 0; i < 6; ++i) {
        seq_t v = (seq_t)factors[i];
        for (idx_t j = 0; j < kstart[i]; ++j) v = v * 2;
        nodes[i].val        = v;
        nodes[i].exp        = kstart[i];
        nodes[i].fact       = factors[i];
        nodes[i].exhausted = false;
    }

    /* generate terms until the requested index is available */
    while (g_len < n) {
        /* select the smallest current value among non‑exhausted nodes */
        size_t min_i = SIZE_MAX;
        seq_t  min_val = 0;
        for (size_t i = 0; i < 6; ++i) {
            if (nodes[i].exhausted) continue;
            if (min_i == SIZE_MAX || nodes[i].val < min_val) {
                min_i   = i;
                min_val = nodes[i].val;
            }
        }

        if (min_i == SIZE_MAX) break;  /* no more values that fit */

        /* add to sequence if not a duplicate */
        if (g_len == 0 || min_val > g_seq[g_len - 1]) {
            append_value(min_val);
        }

        /* advance the chosen progression, detecting overflow */
        seq_t next_val = nodes[min_i].val * 2;
        if (next_val < nodes[min_i].val) {     /* wrap‑around indicates overflow */
            nodes[min_i].exhausted = true;
        } else {
            nodes[min_i].val = next_val;
            nodes[min_i].exp += 1;
        }
    }

    return g_seq[n - 1];
}
