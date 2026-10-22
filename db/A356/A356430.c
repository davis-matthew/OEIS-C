/*
 * A356430  a(1) = 1, a(2) = 2; for n > 2, a(n) is the smallest positive
 *          number not occurring earlier that shares a factor with the number
 *          of divisors of a(n-1).
 * https://oeis.org/A356430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   divisor_count_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2; for n>2, a(n) is the smallest\n\
unused positive integer that shares a non‑trivial gcd with\n\
divisor_count(a(n‑1)). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/gcd.h"
#include "../utils/divisor_count_u64.h"

/* ------------------------------------------------------------------ */
/*  State for lazy, memoised generation                               */
/* ------------------------------------------------------------------ */
static idx_t  max_computed = 0;   /* largest index computed so far           */
static seq_t *seq          = NULL;/* 1‑based array of values                 */
static size_t seq_cap      = 0;   /* number of seq slots currently allocated */

static bool *used         = NULL;/* used[i] true if value i has appeared     */
static size_t used_cap    = 0;   /* size of used array                       */

/* ------------------------------------------------------------------ */
/*  Resize seq array to hold at least 'needed' elements                */
/* ------------------------------------------------------------------ */
static void enlarge_seq(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq = tmp;
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Resize used array to hold at least 'needed' indices                 */
/* ------------------------------------------------------------------ */
static void enlarge_used(size_t needed)
{
    if (needed <= used_cap) return;
    size_t new_cap = used_cap ? used_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    bool *tmp = (bool*)realloc(used, new_cap * sizeof(bool));
    if (!tmp) exit(EXIT_FAILURE);
    /* zero the newly allocated part */
    memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = tmp;
    used_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Compute a(n)                                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    /* Generate terms up to n if not yet done */
    while (max_computed < n)
    {
        idx_t i = max_computed + 1;          /* next index to compute */

        seq_t val;
        if (i == 1)
            val = (seq_t)1;
        else if (i == 2)
            val = (seq_t)2;
        else
        {
            /* Number of divisors of the previous term */
            idx_t prev = (idx_t)seq[i - 1];
            idx_t d    = divisor_count_u64(prev);   /* small integer */

            /* Search smallest unused positive integer sharing a factor with d */
            idx_t cand = 1;
            for (;; ++cand)
            {
                enlarge_used((size_t)cand + 1);
                if (!used[cand] && gcd((seq_t)cand, (seq_t)d) > 1)
                    break;
            }
            val = (seq_t)cand;
            used[(size_t)cand] = true;
        }

        /* Ensure storage for this term */
        enlarge_seq(i + 1);
        seq[i] = val;

        /* Mark the value as used for future searches */
        enlarge_used((size_t)val + 1);
        used[(size_t)val] = true;

        max_computed = i;
    }

    return seq[n];
}
