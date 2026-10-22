/*
 * A366091  a(n) is the number of ways to write n = i^2 + 2*j^2 + 3*k^2 with
 *          i,j,k >= 0.
 * https://oeis.org/A366091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (i,j,k) ≥ 0 with i^2 + 2*j^2 + 3*k^2 = n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following implementation uses a simple exhaustive enumeration
   with memoisation: the first call with argument n pre‑computes all
   values a(0)…a(n) and stores them in a static array. Subsequent calls
   with a larger n extend the array by recomputing from scratch; this
   keeps the code simple and fast enough for n up to about 10 000. */

#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"
#include <string.h>          /* for memset */

/* Static cache of counts; counts[i] = a(i).  On the first call a() the
   array is allocated and all values up to n are computed.  If a later
   call requests a larger n the whole table is recomputed up to the new
   maximal index. */
static seq_t *counts = NULL;
static idx_t  precomputed_max = 0;

/* Recompute all a(k) for 0 ≤ k ≤ max_n.  The table is allocated fresh
   and zero‑initialised. */
static void compute_up_to(idx_t max_n)
{
    /* free existing table if any */
    if (counts != NULL)
        seq_free_array(counts);

    /* allocate new table (max_n+1) elements */
    counts = alloc_seq_array((size_t)(max_n + 1));
    /* zero the array – alloc_seq_array does not guarantee zero‑initialisation */
    memset(counts, 0, (size_t)(max_n + 1) * sizeof(seq_t));

    precomputed_max = max_n;

    /* Enumerate all admissible triples (i,j,k).  Each loop uses
       integer arithmetic only. */
    for (idx_t i = 0; i * i <= max_n; ++i) {
        idx_t i2 = i * i;
        idx_t max_j2 = (max_n - i2) / 2;
        for (idx_t j = 0; j * j <= max_j2; ++j) {
            idx_t j2 = j * j;
            idx_t rem = max_n - i2 - 2 * j2;
            if (rem < 0) break;            /* safety, although loop condition covers this */
            idx_t max_k2 = rem / 3;
            for (idx_t k = 0; k * k <= max_k2; ++k) {
                idx_t val = i2 + 2 * j2 + 3 * k * k;  /* val ≤ max_n by construction */
                counts[(size_t)val]++;              /* record a representation */
            }
        }
    }
}

/* Public API required by the build */
seq_t a(idx_t n)
{
    if (counts == NULL || n > precomputed_max)
        compute_up_to(n);
    return counts[(size_t)n];
}
