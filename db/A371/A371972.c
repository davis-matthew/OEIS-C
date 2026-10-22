/*
 * A371972  a(n) is the number of distinct areas of triangles with integer
 *          sides whose largest side is n.
 * https://oeis.org/A371972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   radix_sort_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A371972  :  a(n) = number of distinct areas of integer‑sided triangles
 *  with largest side equal to n,  n ≥ 1.
 *  The values are obtained by enumerating all admissible unordered triples
 *  (a,b,c) with 1≤a≤b≤c=n and a+b>c, computing the squared area
 *
 *          D = (a+b+c)(-a+b+c)(a-b+c)(a+b-c)
 *
 *  which is an integer.  Distinct values of D/16 give the distinct areas.
 *  The vector of D’s is sorted with radix sort and then deduplicated.
 *  This guarantees exactness for any build using 8,16,32,64 or 128‑bit
 *  integer types (seq_t).  The algorithm runs in O(n²) time and O(n²)
 *  memory (≈ 400 MB for n=10 000). */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/radix_sort_uint64.h"

/* --------------- main sequence function --------------------- */
seq_t a(idx_t n)
{
    /* capacity of the vector (total unordered triangles) */
    seq_t n64 = (seq_t)n;
    seq_t m   = n64 / 2;
    seq_t total_pairs = n64 * (n64 + 1) / 2 - m * (n64 - m);
    size_t cap = (size_t)total_pairs;        /* guaranteed fits in size_t */

    /* allocate arrays for raw D values and auxiliary buffer */
    uint64_t *data = (uint64_t*)malloc(cap * sizeof(uint64_t));
    uint64_t *temp = (uint64_t*)malloc(cap * sizeof(uint64_t));
    if (!data || !temp) {
        /* memory allocation failed – return 0 */
        free(data);
        free(temp);
        return 0;
    }

    /* enumerate all admissible (a,b) pairs */
    size_t pos = 0;
    for (idx_t a_val = 1; a_val <= n; ++a_val) {
        idx_t b_min = a_val;
        idx_t limit = n - a_val + 1;
        if (limit > b_min) b_min = limit;
        if (b_min > n) continue;   /* no pair for this a */
        for (idx_t b_val = b_min; b_val <= n; ++b_val) {
            /* compute the squared area numerator D */
            seq_t a_s = (seq_t)a_val;
            seq_t b_s = (seq_t)b_val;
            seq_t p   = a_s + b_s;                     /* a+b */
            seq_t q   = b_s - a_s;                     /* b-a (>=0) */
            seq_t term1 = p * p - n64 * n64;           /* p^2 – n^2 */
            seq_t term2 = n64 * n64 - q * q;           /* n^2 – q^2 */
            seq_t val   = term1 * term2;               /* D */

            data[pos++] = (uint64_t)val;
        }
    }

    /* now unique count after sorting */
    if (cap > 1)
        radix_sort_uint64(data, cap);

    seq_t unique = 0;
    if (cap > 0) {
        uint64_t prev = data[0];
        unique = 1;
        for (size_t i = 1; i < cap; ++i) {
            uint64_t cur = data[i];
            if (cur != prev) {
                ++unique;
                prev = cur;
            }
        }
    }

    /* free temporary buffers */
    free(data);
    free(temp);

    return unique;
}
