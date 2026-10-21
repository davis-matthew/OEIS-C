/*
 * A277147  Lexicographically least sequence of nonnegative integers that
 *          avoids 7/5-powers.
 * https://oeis.org/A277147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9326, u16 n <= 9326, u32 n <= 9326, u64 n <= 9326,
 *            u128 n <= 9326, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically least sequence avoiding 7/5-powers (A277147)
   The sequence is built greedily: for each n, append the smallest
   non‑negative integer that does not create a word of the form
   u + prefix(u,2k) (|u|=5k) anywhere in the sequence.  This
   implementation checks only patterns that involve the freshly
   appended element, avoiding any out‑of‑bounds accesses.  The cache
   is kept in an expanding array (C) or vector (C++).  The code is
   fully portable to C11 and C++17. */

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/* --------------------------------------------------------------------- */
/* Check whether appending 'cand' would create a forbidden 7/5‑power.
   seq  : pointer to current sequence of length 'len' (before adding cand)
   len  : current number of elements in seq
   cand : value to be appended.
   Returns true if the new sequence would contain a forbidden pattern. */
static bool creates_power(const seq_t *seq, size_t len, seq_t cand)
{
    size_t new_len = len + 1;                    /* length after append */
    for (size_t k = 1; 7 * k <= new_len; ++k) {  /* examine all possible k */
        size_t start = new_len - 7 * k;          /* beginning of a possible block */

        /* Compare the first 5k elements (left side)
           with the last 2k using either existing seq elements
           or the candidate if the index is the new element.   */
        size_t j;
        for (j = 0; j < 2 * k; ++j) {
            seq_t left  = seq[start + j];            /* index < len always */
            seq_t right;
            size_t idx2 = start + 5 * k + j;         /* may equal 'len' */
            if (idx2 < len)
                right = seq[idx2];
            else
                right = cand;                        /* new element */

            if (left != right)
                break;                               /* not a power */
        }
        if (j == 2 * k)          /* all 2k positions matched */
            return true;          /* forbidden pattern would appear */
    }
    return false;                /* safe to append cand */
}

/* --------------------------------------------------------------------- */
/* C implementation: dynamic array cache -------------------------------- */
#ifndef __cplusplus
static seq_t *a_cache = NULL;   /* array of computed terms */
static size_t a_len   = 0;      /* number of terms stored      */
static size_t a_cap   = 0;      /* allocated capacity          */

static void add_term(void)
{
    seq_t cand = 0;
    while (creates_power(a_cache, a_len, cand))
        ++cand;

    if (a_len == a_cap) {        /* grow array if needed */
        size_t new_cap = a_cap ? a_cap * 2 : 128;
        seq_t *tmp = realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();       /* allocation failed */
        a_cache = tmp;
        a_cap   = new_cap;
    }
    a_cache[a_len++] = cand;     /* store new term */
}
#else

/* --------------------------------------------------------------------- */
/* C++ implementation: std::vector cache -------------------------------- */
static std::vector<seq_t> a_vec;   /* vector of computed terms */
static size_t a_len   = 0;         /* keep explicit count (matches size) */

static void add_term(void)
{
    seq_t cand = 0;
    while (creates_power(a_vec.data(), a_vec.size(), cand))
        ++cand;

    a_vec.push_back(cand);
    ++a_len;                     /* stay in sync with vector size */
}
#endif

/* --------------------------------------------------------------------- */
/* Public entry point --------------------------------------------------- */
seq_t a(idx_t n)
{
    size_t target = (size_t)n;

    if (target < a_len) {
#ifdef __cplusplus
        return a_vec[target];
#else
        return a_cache[target];
#endif
    }

    while (a_len <= target)
        add_term();               /* compute terms up to required index */

#ifdef __cplusplus
    return a_vec[target];
#else
    return a_cache[target];
#endif
}
