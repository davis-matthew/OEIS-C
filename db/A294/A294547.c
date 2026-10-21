/*
 * A294547  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) + 2n, where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n)) and
 *          (b(n)) are increasing complementary sequences.
 * https://oeis.org/A294547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-1) + 2n, * a(0)=1, a(1)=2, b(0)=3.
 *            * (a(n)) and (b(n)) are the unique increasing complementary
 *            sequences * satisfying this recurrence. This function memoises
 *            the values in * static dynamic arrays and can be called in any
 *            order. It uses only * standard C99/C++17 header...
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Global static caches for the two complementary sequences. */
static seq_t *a_cache = NULL; /* cache for a(n) */
static size_t a_size  = 0;    /* number of values stored in a_cache */
static size_t a_cap   = 0;    /* current capacity of a_cache */

static seq_t *b_cache = NULL; /* cache for b(n) */
static size_t b_size  = 0;    /* number of values stored in b_cache */
static size_t b_cap   = 0;    /* current capacity of b_cache */

/* The smallest integer that is not yet assigned to a or b. */
static seq_t next_candidate = 1;
/* Indices used to scan the ordered arrays while searching for the
 * next unused integer. */
static size_t a_scan = 0;
static size_t b_scan = 0;

/* Ensure that a_cache can hold at least 'needed' elements. */
static void ensure_a_capacity(size_t needed)
{
    if (needed <= a_cap) return;
    size_t new_cap = a_cap ? a_cap * 2 : 4;
    while (new_cap < needed) new_cap <<= 1;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cap = new_cap;
}

/* Ensure that b_cache can hold at least 'needed' elements. */
static void ensure_b_capacity(size_t needed)
{
    if (needed <= b_cap) return;
    size_t new_cap = b_cap ? b_cap * 2 : 4;
    while (new_cap < needed) new_cap <<= 1;
    b_cache = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
    b_cap = new_cap;
}

/* Return the next unused integer that will become the next b(n). */
static seq_t next_b_value(void)
{
    /* Advance scan indices to values ≥ next_candidate. */
    while (a_scan < a_size && a_cache[a_scan] < next_candidate) ++a_scan;
    while (b_scan < b_size && b_cache[b_scan] < next_candidate) ++b_scan;

    for (;;)
    {
        /* Skip if the candidate is already in a. */
        if (a_scan < a_size && a_cache[a_scan] == next_candidate)
        {
            ++next_candidate;
            ++a_scan;
            continue;
        }
        /* Skip if the candidate is already in b. */
        if (b_scan < b_size && b_cache[b_scan] == next_candidate)
        {
            ++next_candidate;
            ++b_scan;
            continue;
        }
        /* Candidate is free – use it and advance. */
        seq_t val = next_candidate;
        ++next_candidate;
        return val;
    }
}

/* Compute the complementary sequences up to index 'n'.  The function
 * is re‑entrant and may be called in any order.  It memoises all
 * intermediate values. */
seq_t a(idx_t n)
{
    /* One‑time initialization of the static arrays and seed values. */
    if (a_cache == NULL)
    {
        /* a(0)=1, a(1)=2 */
        ensure_a_capacity(2);
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)2;
        a_size = 2;

        /* b(0)=3 */
        ensure_b_capacity(1);
        b_cache[0] = (seq_t)3;
        b_size = 1;

        /* The first candidate that cannot be 1,2 or 3 is 4. */
        next_candidate = 4;
    }

    /* If the requested index is already cached, return it. */
    if ((size_t)n < a_size)
        return a_cache[n];

    /* Compute all missing a-values up to index n. */
    for (size_t idx = a_size; idx <= (size_t)n; ++idx)
    {
        /* Ensure that b(idx-1) exists. */
        while (b_size <= idx - 1)
        {
            ensure_b_capacity(b_size + 1);
            b_cache[b_size++] = next_b_value();
        }

        /* Compute a(idx) using the recurrence. */
        seq_t idx_seq = (seq_t)idx;
        seq_t val = a_cache[idx-1] + a_cache[idx-2] + b_cache[idx-1]
                    + (seq_t)2 * idx_seq;

        /* Store the computed value. */
        if (idx >= a_cap) ensure_a_capacity(idx + 1);
        a_cache[idx] = val;
        ++a_size;
    }

    return a_cache[n];
}
