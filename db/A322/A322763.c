/*
 * A322763  Irregular triangle read by rows: to get row n, take partitions of
 *          n ordered as in A080577, and in each partition, change each j-th
 *          occurrence of k to j; use uncompressed notation as in A080577.
 * https://oeis.org/A322763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, base
 * Formula:   not stated
 * Range:     u8 n <= 9993, u16 n <= 9993, u32 n <= 9993, u64 n <= 9993,
 *            u128 n <= 9993, big any n
 * Helpers:   append_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A322763
    Irregular triangle read by rows.
    For each integer m (m ≥ 1), the partitions of m are listed in
    lexicographic order of non‑increasing parts (A080577).  For each
    partition, the j‑th occurrence of a part k is replaced by the digit j
    (the occurrence index).  The resulting digits, in the order of the
    partitions, are concatenated to form the infinite sequence a(n),
    indexed from 1.  The routine below generates the sequence lazily,
    caching already computed terms so that a(idx_t n) can be called in
    any order. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/append_digit.h"

static seq_t  *seq_cache      = NULL;   /* cache of computed terms */
static idx_t  seq_cache_len  = 0;        /* number of terms stored    */
static idx_t  seq_cache_cap  = 0;        /* allocated capacity       */
static idx_t  next_row       = 1;        /* next integer m to process */

/* Generate all partitions of the integer m in lexicographic non‑increasing
   order and append their transformed digits to the cache. */
static void generate_row(idx_t m,
                        seq_t **cache, idx_t *len, idx_t *cap)
{
    /* Allocate work space on the heap to avoid deep recursion. */
    int *parts  = (int *)malloc((m + 1) * sizeof(int));
    int *counts = (int *)malloc((m + 1) * sizeof(int));
    if (!parts || !counts) abort();

    for (idx_t i = 0; i <= m; ++i) counts[i] = 0;

    /* Initialise first partition: [m] */
    parts[0] = (int)m;
    idx_t k = 1;                    // current number of parts

    for (;;) {
        /* Reset counts for this partition */
        for (idx_t i = 0; i <= m; ++i) counts[i] = 0;

        /* Emit digits of the current partition */
        for (idx_t idx = 0; idx < k; ++idx) {
            int part = parts[idx];
            int occ  = ++counts[part];          /* j‑th occurrence */
            append_digit(cache, len, cap, (seq_t)occ);
        }

        /* Generate next partition in lexicographic order */
        /* Find rightmost part > 1 */
        idx_t j = k;
        while (j > 0 && parts[j-1] == 1) j--;
        if (j == 0) break;              /* no more partitions */

        /* Decrement that part by 1 */
        parts[j-1]--;
        int rem = (int)(k - j + 1);     /* remaining units to redistribute */

        /* Reset the tail of the partition */
        k = j;
        while (rem > parts[j-1]) {
            parts[k++] = parts[j-1];
            rem -= parts[j-1];
        }
        parts[k++] = rem;
    }

    free(parts);
    free(counts);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but safe fallback */

    /* Extend cache until it contains term n */
    while (seq_cache_len < n) {
        generate_row(next_row, &seq_cache, &seq_cache_len, &seq_cache_cap);
        next_row++;
    }
    return seq_cache[n - 1];
}
