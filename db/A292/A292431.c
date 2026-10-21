/*
 * A292431  Numbers n with record numbers of unordered triples {a, b, c} of
 *          distinct positive integers from 1 to n such that a*b = c*n.
 * https://oeis.org/A292431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n that attains a record number of unordered distinct
 *            triples {a,b,c} ⊆ {1,…,n} with a*b = c*n. The sequence starts
 *            with a(1)=0 (trivial record), then records are 6,10,12,18,
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Assumes the following types are defined by the build environment:
   typedef <unsigned integer type> seq_t;   // e.g. uint64_t, __int128, ...
   typedef <manually chosen unsigned integer> idx_t;  // typically uint64_t
*/

#include <stdlib.h>
#include <stdbool.h>

/* Simple Euclidean gcd for idx_t values */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Count the number of unordered distinct triples {a,b,c} with a*b = c*n */
static seq_t count_triples(idx_t n)
{
    seq_t total = 0;
    for (idx_t a = 1; a < n; ++a) {
        idx_t d = gcd_idx(a, n);
        if (d <= 1) continue;          /* d==1 contributes nothing */
        idx_t m = n / d;                /* m = n/d */
        idx_t q = a / m;                /* floor(a/m) */
        idx_t contrib = d - 1 - q;      /* may be zero or negative */
        if (contrib > 0) total += contrib;
    }
    return total;
}

/* Static storage for the record positions */
static seq_t *record_vals = NULL;   /* array of n values that are records */
static size_t record_count = 0;     /* number of recorded positions */
static size_t record_cap   = 0;
static idx_t next_n = 1;           /* next n value to evaluate */
static seq_t last_max = 0;         /* maximum count seen so far */

/* Grow the record array when needed */
static void ensure_record_cap(size_t needed)
{
    if (needed <= record_cap) return;
    size_t new_cap = record_cap ? record_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(record_vals, new_cap * sizeof(seq_t));
    record_vals = new_ptr;
    record_cap = new_cap;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* sequence offset is 1, but return 0 for safety */

    /* Initialize record list with the trivial 0 record */
    if (record_count == 0) {
        ensure_record_cap(1);
        record_vals[0] = (seq_t)0;
        record_count = 1;
        last_max = 0;
        next_n = 1;
    }

    /* Generate records until we have at least n records */
    while (record_count < (size_t)n) {
        seq_t cur_count = count_triples(next_n);
        if (cur_count > last_max) {
            /* New record found */
            ensure_record_cap(record_count + 1);
            record_vals[record_count++] = (seq_t)next_n;
            last_max = cur_count;
        }
        ++next_n;
    }

    return record_vals[n - 1];
}
