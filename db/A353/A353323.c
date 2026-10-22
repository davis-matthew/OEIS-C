/*
 * A353323  A variant of Van Eck's sequence where we only consider powers of
 *          3: for n >= 0, if a(n) = a(n-3^e) for some e, take the least such
 *          e and set a(n+1) = 3^e; otherwise a(n+1) = 0. Start with a(1) = 0.
 * https://oeis.org/A353323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 0;
   For n ≥ 1, let e be the smallest non‑negative integer such that 3^e ≤ n−1
   and a(n) == a(n−3^e).  If such an e exists set a(n+1) = 3^e; otherwise
   a(n+1) = 0.  The sequence is 1‑based. */

#include <stdlib.h>   /* malloc, realloc, exit */

static seq_t *seq = NULL;   /* 1‑based array of terms            */
static size_t sz  = 0;      /* highest index that has been computed */
static size_t cap = 0;      /* allocated array size (number of slots) */

/* Ensure capacity for at least 'needed' indices (1‑based). 
   New entries beyond the old capacity are zeroed. */
static void ensure_capacity(idx_t needed)
{
    if (needed <= (idx_t)cap) return;          /* already enough */

    size_t newcap = (cap ? cap : 2);
    while ((idx_t)newcap < needed) newcap <<= 1;

    size_t oldcap = cap;
    seq_t *newptr = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
    if (!newptr) exit(1);                      /* fatal if allocation fails */

    /* Zero newly allocated slots only. */
    for (size_t i = oldcap; i < newcap; ++i) newptr[i] = (seq_t)0;

    seq = newptr;
    cap = newcap;
}

/* Returns the n‑th term (n ≥ 1).  Calls may come in any order. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                 /* undefined for n<1 */

    /* Lazily initialize the first term on the first call. */
    if (!seq) {
        ensure_capacity(2);                     /* need indices 0 and 1 */
        seq[1] = (seq_t)0;                      /* a(1) = 0 */
        sz = 1;
    }

    /* Compute missing terms up to index n. */
    if ((idx_t)sz < n) {
        ensure_capacity(n + 1);                 /* need indices 0…n */

        for (idx_t i = sz; i < n; ++i) {
            seq_t out = (seq_t)0;               /* default value */
            idx_t pow = 1;                      /* 3^e */

            /* Iterate over powers of 3 with 3^e ≤ i−1. */
            while (pow <= i - 1) {
                if (seq[(size_t)i] == seq[(size_t)(i - pow)]) {
                    out = (seq_t)pow;           /* 3^e found */
                    break;                       /* smallest e satisfied */
                }

                /* Stop if next multiplication would overflow idx_t */
                if (pow > ((idx_t)-1) / 3) break;
                pow *= 3;
            }
            seq[(size_t)(i + 1)] = out;         /* store a(i+1) */
        }
        sz = n;
    }

    return seq[(size_t)n];
}
