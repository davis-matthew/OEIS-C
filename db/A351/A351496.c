/*
 * A351496  Lexicographically earliest infinite sequence of distinct positive
 *          numbers such that, for n>2, a(n) has a common factor with the
 *          largest previous term but not with the second largest previous
 *          term.
 * https://oeis.org/A351496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   gcd, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A351496: lexicographically earliest infinite sequence of
 * distinct positive numbers such that, for n>2, a(n) shares a common
 * factor with the largest previous term but not with the second largest
 * previous term.
 *
 * The algorithm builds the sequence iteratively, caching all terms in
 * an internal array.  Each new term is chosen as the smallest unused
 * positive integer that satisfies the gcd conditions and the additional
 * prime‑factor constraints described in the OEIS entry.
 *
 * Author: ChatGPT
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/gcd.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_alloc_new_array.h"

/* ------------------------------------------------------------------ */
/*  Internal state                                                    */
/* ------------------------------------------------------------------ */
static seq_t   *seq   = NULL;   /* array of sequence values          */
static size_t  seq_cap = 0;     /* allocated capacity                */
static size_t  seq_len = 0;     /* number of values stored           */
static seq_t   L = 0;           /* largest previous term             */
static seq_t   S = 0;           /* second largest previous term      */

/* ------------------------------------------------------------------ */
/*  Utility functions                                                 */
/* ------------------------------------------------------------------ */

/* Resize seq[] to hold at least 'need' elements.  seq_cap is updated. */
static void ensure_seq_capacity(size_t need)
{
    if (seq_cap >= need)
        return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;

    seq = seq_realloc(seq, seq_cap, new_cap);  /* seq_realloc handles realloc/alloc */
    seq_cap = new_cap;
}

/* Check whether 'val' is already in seq[0 .. seq_len-1] */
static bool used(seq_t val)
{
    for (size_t i = 0; i < seq_len; ++i) {
        if (seq[i] == val)
            return true;
    }
    return false;
}

/* Initialise the sequence with the first three terms.  This is called
 * once by the first invocation of a(). */
static void init_sequence(void)
{
    if (seq_len != 0)
        return;  /* already initialised */

    /* Allocate room for at least 3 terms. */
    ensure_seq_capacity(3);

    seq[0] = 1;
    seq[1] = 2;
    seq[2] = 6;
    seq_len = 3;

    /* Set largest (L) and second largest (S).  The largest of the
     * initial three terms is 6, the second largest is 2. */
    L = 6;
    S = 2;
}

/* ------------------------------------------------------------------ */
/*  Sequence generator                                                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n==0, but return 0 to avoid crash */

    init_sequence();

    /* Return cached value if already computed. */
    if ((size_t)n <= seq_len)
        return seq[(size_t)n - 1];

    /* Grow the sequence up to the requested index. */
    for (idx_t idx = seq_len + 1; idx <= n; ++idx) {
        seq_t candidate = 1;

        /* Find the smallest valid unused integer. */
        for (;;) {
            if (used(candidate))
                { ++candidate; continue; }

            /* Must share a factor with the largest previous term. */
            seq_t g1 = gcd(candidate, L);
            if (g1 <= 1)
                { ++candidate; continue; }

            /* Must NOT share a factor with the second largest previous term. */
            seq_t g2 = gcd(candidate, S);
            if (g2 != 1)
                { ++candidate; continue; }

            /* Additional prime‑factor constraints. */
            if (candidate > L) {
                seq_t rem = candidate / g1;
                if (rem <= 1 || gcd(rem, L) != 1) {
                    ++candidate;
                    continue;  /* candidate has no new prime relative to L */
                }
            } else if (candidate > S && candidate < L) {
                if (L / g1 <= 1) {
                    ++candidate;
                    continue;  /* L would be fully contained in candidate */
                }
                /* No further check needed for candidate < S. */
            }

            /* Accept this candidate. */
            seq_len++;
            ensure_seq_capacity(seq_len + 1);
            seq[seq_len - 1] = candidate;

            /* Update largest (L) and second largest (S). */
            if (candidate > L) {
                S = L;
                L = candidate;
            } else if (candidate > S) {
                S = candidate;
            }

            break;   /* next term computed, proceed to next index */
        }
    }

    return seq[(size_t)n - 1];
}
