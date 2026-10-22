/*
 * A354420  Lexicographically earliest infinite sequence of distinct positive
 *          numbers such that, for n>3, a(n) has a common factor with a(n-2),
 *          shares a 1-bit in its binary expansion with a(n-2), has no common
 *          factor with a(n-1), and does not share a 1-bit in its binary
 *          expansion with a(n-1).
 * https://oeis.org/A354420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: lexicographically earliest infinite sequence of distinct
 * positive integers in which for n>3, a(n) shares a common factor and a
 * common 1‑bit in binary with a(n−2), while sharing no common factor and
 * no 1‑bits with a(n−1). The first terms are 1,2,5,18,65,6,25,4,…
 *
 * The algorithm memoises all previously computed terms and for each new
 * index performs a local search for the smallest unused positive integer
 * satisfying the four conditions.  Used numbers are recorded in a
 * bitmap whose size grows as necessary.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"          /* gcd(seq_t, seq_t) */

/* ------------------------------------------------------------------ */
/* Global memoisation state                                           */
/* ------------------------------------------------------------------ */
static seq_t   *seq_vec   = NULL;   /* 0‑based array: seq_vec[n‑1] = a(n)   */
static size_t   seq_cap   = 0;       /* current capacity of seq_vec          */
static size_t   seq_len   = 0;       /* number of stored terms (>= seq_len)   */

static uint8_t *used_bitmap = NULL; /* bitmap of used indices                */
static size_t  used_cap    = 0;      /* current capacity of used_bitmap       */

/* ------------------------------------------------------------------ */
/* Utility: grow the sequence vector to at least 'needed' slots      */
/* ------------------------------------------------------------------ */
static void grow_seq_cap(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_vec, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    seq_vec = tmp;
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Utility: grow the used‑bitmap to at least 'needed' entries         */
/* ------------------------------------------------------------------ */
static void grow_used_cap(size_t needed)
{
    if (needed <= used_cap)
        return;
    size_t new_cap = used_cap ? used_cap * 2 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(used_bitmap, new_cap);
    if (!tmp)
        abort();
    /* Zero the newly allocated part */
    memset(tmp + used_cap, 0, new_cap - used_cap);
    used_bitmap = tmp;
    used_cap    = new_cap;
}

/* ------------------------------------------------------------------ */
/* Helper: test whether value 'v' has already been used                */
/* ------------------------------------------------------------------ */
static inline bool test_used(idx_t v)
{
    return v < used_cap && used_bitmap[v];
}

/* ------------------------------------------------------------------ */
/* Helper: mark value 'v' as used                                     */
/* ------------------------------------------------------------------ */
static inline void set_used(idx_t v)
{
    if (v >= used_cap)
        grow_used_cap(v + 1);
    used_bitmap[v] = 1;
}

/* ------------------------------------------------------------------ */
/* Return the n‑th term (1‑based).                                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined but avoid crash */

    /* Ensure we have computed terms up to 'n' */
    if (n > seq_len) {
        /* Allocate space for up to 'n' terms */
        grow_seq_cap(n);

        /* Compute missing terms sequentially */
        while (seq_len < n) {
            size_t idx = seq_len + 1;    /* current 1‑based index to compute */
            seq_t val;

            /* Initial known terms */
            if (idx == 1)
                val = (seq_t)1;
            else if (idx == 2)
                val = (seq_t)2;
            else if (idx == 3)
                val = (seq_t)5;
            else if (idx == 4)
                val = (seq_t)18;
            else {
                /* Search for the smallest unused positive integer that
                   satisfies the four constraints relative to a(n‑1) and
                   a(n‑2).                                 */
                seq_t prev1 = seq_vec[idx - 2]; /* a(n-1)  */
                seq_t prev2 = seq_vec[idx - 3]; /* a(n-2)  */
                seq_t cand;
                idx_t  cand_i;

                for (cand_i = 1; ; ++cand_i) {
                    if (test_used(cand_i))
                        continue;               /* already used elsewhere */

                    cand = (seq_t)cand_i;

                    /* Condition 1: share factor > 1 with a(n‑2)      */
                    if (gcd(cand, prev2) <= (seq_t)1)
                        continue;

                    /* Condition 2: share at least one 1‑bit with a(n‑2) */
                    if ((cand & prev2) == (seq_t)0)
                        continue;

                    /* Condition 3: coprime with a(n‑1)                 */
                    if (gcd(cand, prev1) != (seq_t)1)
                        continue;

                    /* Condition 4: no common 1‑bits with a(n‑1)       */
                    if ((cand & prev1) != (seq_t)0)
                        continue;

                    /* Candidate found */
                    set_used(cand_i);  /* record that this integer has been used */
                    val = cand;
                    break;
                }
            }

            seq_vec[seq_len]   = val;      /* store the term */
            set_used((idx_t)val);          /* safety: mark the value itself     */
            ++seq_len;
        }
    }

    return seq_vec[n - 1];
}
