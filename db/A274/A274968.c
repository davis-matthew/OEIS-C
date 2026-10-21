/*
 * A274968  Even numbers n >= 4 which are not m-gonal number for 3 <= m < n.
 * https://oeis.org/A274968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 9514, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A274968:
   Even numbers n ≥ 4 which are not m‑gonal numbers for any 3 ≤ m < n.
   The implementation sieves all m‑gonal numbers with m ≥ 3 and k ≥ 3
   and stores the remaining even numbers.  The sieve is extended lazily
   as higher indices are requested.  The algorithm runs in
   approximately O(n log n) time and uses O(n) memory.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static idx_t seq_len  = 0;              /* number of terms already stored (1‑based) */
static seq_t *seq_cache = NULL;        /* array holding the sequence terms   */
static size_t seq_cap  = 0;             /* current capacity of seq_cache      */

static bool *marked  = NULL;           /* marked[i] == true  if i is an m‑gonal number (k≥3) */
static idx_t marked_cap = 0;           /* capacity of the marked array        */

static idx_t *next_k  = NULL;          /* next k to test for each m             */
static idx_t max_m     = 0;             /* highest m for which next_k is allocated */

static idx_t next_candidate = 4;        /* next even number to consider         */

/* Expand the marked array and the next_k array so that all m‑gonal numbers
   ≤ limit are marked.  The function always uses the current value of
   max_m and expands it if necessary. */
static void expand_limit(idx_t limit)
{
    if (limit < 4) return;              /* no work for the trivial case */

    /* Grow the marked array to cover [0..limit] */
    if (limit >= marked_cap) {
        idx_t new_cap = limit + 1;
        bool *tmp = (bool *)realloc(marked, new_cap * sizeof(bool));
        if (!tmp) abort();
        for (idx_t i = marked_cap; i < new_cap; ++i) tmp[i] = false;
        marked = tmp;
        marked_cap = new_cap;
    }

    /* Determine the largest m that can produce a number ≤ limit with k ≥ 3.
       Minimal k = 3 gives value 3m – 3.  Solve 3m – 3 ≤ limit → m ≤ (limit+3)/3.
       Using integer arithmetic: needed_m = limit/3 + 1. */
    idx_t needed_m = limit / 3 + 1;
    if (needed_m > max_m) {
        idx_t new_m_cap = needed_m + 1;      /* allow index = needed_m */
        idx_t *tmp = (idx_t *)realloc(next_k, new_m_cap * sizeof(idx_t));
        if (!tmp) abort();
        for (idx_t i = max_m + 1; i < new_m_cap; ++i) tmp[i] = 3;
        next_k = tmp;
        max_m = needed_m;
    }

    /* Sieve m‑gonal numbers for all m = 3 … max_m */
    for (idx_t m = 3; m <= max_m; ++m) {
        idx_t k = next_k[m];
        while (1) {
            unsigned __int128 val128;
            if (m == 3) {
                /* Triangular numbers: k(k+1)/2 with k ≥ 3 */
                val128 = (unsigned __int128)k * (k + 1) / 2;
            } else {
                /* General formula: ((m‑2)k² – (m‑4)k)/2  for m ≥ 4 */
                val128 = ((unsigned __int128)(m - 2) * k * k -
                          (unsigned __int128)(m - 4) * k) / 2;
            }
            if (val128 > limit) break;

            idx_t val = (idx_t)val128;
            marked[val] = true;
            next_k[m] = k + 1;
            ++k;
        }
    }
}

/* Ensure seq_cache has space for at least 'needed' elements. */
static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;
    seq_cap = new_cap;
}

/* The requested function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for zero index */

    if (n <= seq_len) return seq_cache[n - 1];

    /* Generate terms up to the requested index */
    while (seq_len < n) {
        /* Make sure the sieve covers the next candidate */
        expand_limit(next_candidate);

        /* If the next even number is not marked, it belongs to the sequence */
        if (!marked[next_candidate]) {
            ensure_seq_capacity(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)next_candidate;
        }
        next_candidate += 2;                  /* advance to the next even number */
    }
    return seq_cache[n - 1];
}
