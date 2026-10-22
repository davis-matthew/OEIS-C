/*
 * A356879  Numbers k such that the sum k^x + k^y can be a square with {x, y}
 *          >= 0.
 * https://oeis.org/A356879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) are the distinct numbers of the form t²‑1 (t≥1) or 2·t²
 *            (t≥0), sorted increasingly. a(0)=0, a(1)=2, a(2)=3, … (OEIS
 *            A356879)
 * Range:     u8 n <= 24, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache     = NULL;      /* cached values of the sequence          */
static size_t cache_len = 0;         /* number of elements currently stored   */
static size_t cache_cap = 0;         /* current capacity of the cache array    */
static idx_t  t1        = 1;         /* next t for t²‑1                         */
static idx_t  t2        = 0;         /* next t for 2·t²                         */

/* Ensure that cache contains at least n+1 terms (0‑based indexing). */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 64;                         /* initial capacity */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len <= (size_t)n) {
        /* Candidates from the two subsequences */
        seq_t cand1 = ((seq_t)t1 * (seq_t)t1) - 1;          /* t²‑1     */
        seq_t cand2 = ((seq_t)2 * (seq_t)t2 * (seq_t)t2);   /* 2·t²     */

        /* Choose the smaller candidate (or handle equality) */
        seq_t next_val;
        int   take1 = 1;   /* 1 → candidate1, 0 → candidate2, 2 → both equal */

        if (cand1 < cand2) {
            next_val = cand1;
            take1 = 1;
        } else if (cand2 < cand1) {
            next_val = cand2;
            take1 = 0;
        } else {               /* cand1 == cand2 */
            next_val = cand1;
            take1 = 2;
        }

        /* Insert the value if it is not a duplicate of the last one */
        if (cache_len == 0 || next_val != cache[cache_len - 1]) {
            if (cache_len == cache_cap) {
                /* Grow cache dynamically */
                size_t new_cap = cache_cap * 2;
                seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache     = new_ptr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_val;
        }

        /* Advance the index(es) that provided the chosen candidate(s) */
        if (take1 == 1)        t1++;
        else if (take1 == 0)   t2++;
        else {                 /* equal - increment both */
            t1++; t2++;
        }
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
