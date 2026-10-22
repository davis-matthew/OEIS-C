/*
 * A387502  Number of debut sums of initial subsequences of the divisors > 1
 *          of n.
 * https://oeis.org/A387502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of sequence A387502:
   a(n) = number of debut sums of initial subsequences of the
   divisors > 1 of n.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Ensure that the bool flag array `sum_seen` can store an entry at
   index `idx` (0‑based).  This array records whether a particular
   partial sum has already appeared in D(m) for some m < current n. */
static bool  *sum_seen    = NULL;
static size_t sum_seen_cap  = 0;

static void ensure_sum_seen_cap(size_t idx)
{
    if (idx < sum_seen_cap) return;
    size_t new_cap = sum_seen_cap ? sum_seen_cap << 1 : 1;
    while (new_cap <= idx) new_cap <<= 1;
    bool *tmp = (bool *)realloc(sum_seen, new_cap * sizeof(bool));
    if (!tmp) abort();
    memset(tmp + sum_seen_cap, 0, (new_cap - sum_seen_cap) * sizeof(bool));
    sum_seen = tmp;
    sum_seen_cap = new_cap;
}

/* Cached values of a(n).  a_cache[0] is unused; index i stores a(i). */
static seq_t *a_cache      = NULL;
static size_t a_cache_cap  = 0;
static size_t a_computed   = 0;   /* largest n already processed */

static void ensure_a_cache_cap(size_t idx)
{
    if (idx < a_cache_cap) return;
    size_t new_cap = a_cache_cap ? a_cache_cap << 1 : 1;
    while (new_cap <= idx) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    /* Initialise new elements to zero.  This works for integer
       types and for big‑int classes that have a default constructor. */
    seq_t zero = 0;
    for (size_t i = a_cache_cap; i < new_cap; ++i) tmp[i] = zero;
    a_cache = tmp;
    a_cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* defined only for n >= 1 */

    /* Return already cached result if available. */
    if ((size_t)n <= a_computed) return a_cache[(size_t)n];

    /* Compute a(m) for all m in (a_computed, n] */
    for (idx_t m = a_computed + 1; m <= n; ++m) {
        /* Collect divisors > 1 of m.  `small` holds divisors up to
           sqrt(m) in ascending order; `large` holds the complementary
           divisors in descending order. */
        uint64_t small[128];
        uint64_t large[128];
        size_t scnt = 0, lcnt = 0;

        uint64_t mm = (uint64_t)m;
        for (uint64_t d = 2; d <= mm / d; ++d) {
            if (mm % d == 0) {
                small[scnt++] = d;
                if (d != mm / d) large[lcnt++] = mm / d;
            }
        }

        seq_t count = 0;
        uint64_t sum = 0;

        /* Process small divisors (ascending). */
        for (size_t i = 0; i < scnt; ++i) {
            sum += small[i];
            ensure_sum_seen_cap((size_t)sum);
            if (!sum_seen[(size_t)sum]) {
                sum_seen[(size_t)sum] = true;
                ++count;
            }
        }

        /* Process large divisors (ascending): reverse stored order. */
        for (size_t i = lcnt; i-- > 0; ) {
            sum += large[i];
            ensure_sum_seen_cap((size_t)sum);
            if (!sum_seen[(size_t)sum]) {
                sum_seen[(size_t)sum] = true;
                ++count;
            }
        }

        /* Finally add m itself (it is always > 1 for m > 1). */
        if (mm > 1) {
            sum += mm;
            ensure_sum_seen_cap((size_t)sum);
            if (!sum_seen[(size_t)sum]) {
                sum_seen[(size_t)sum] = true;
                ++count;
            }
        }

        /* Store the computed value. */
        ensure_a_cache_cap((size_t)m);
        a_cache[(size_t)m] = count;
    }

    a_computed = n;
    return a_cache[(size_t)n];
}
