/*
 * A295283  a(n) = 1 + number of distinct earlier terms that have common one
 *          bits with n in binary representation.
 * https://oeis.org/A295283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 + |{ a(k) : k < n, (a(k) & n) ≠ 0 }|
 * Range:     u8 n <= 8447, u16 n <= 8447, u32 n <= 8447, u64 n <= 8447,
 *            u128 n <= 8447, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Persistent state for memoisation */
    static idx_t max_computed = 0;      /* largest n whose a(n) has been computed */
    static seq_t *a_vals      = NULL;   /* table of a(1..max_computed)  */
    static size_t a_cap       = 0;      /* current capacity of a_vals (index 0 unused) */
    /* used for deduplication of a(k) values that match the current n */
    static uint64_t *seen_ts = NULL;    /* timestamp of last appearance of a value */
    static size_t seen_cap = 0;         /* current capacity of seen_ts array */
    static uint64_t cur_ts = 1;         /* current timestamp counter */

    /* Allocate or grow a_vals as needed */
    if (max_computed < n) {
        if (a_cap <= (size_t)n) {
            size_t new_cap = (n > a_cap * 2) ? n + 1 : a_cap * 2;
            seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            a_vals = tmp;
            a_cap = new_cap;
        }

        /* Allocate or grow seen_ts array as needed */
        if (seen_cap <= max_computed) {
            size_t new_seen_cap = (seen_cap == 0) ? 1 : seen_cap;
            while (new_seen_cap <= max_computed) new_seen_cap <<= 1;
            uint64_t *tmp = (uint64_t *)realloc(seen_ts, new_seen_cap * sizeof(uint64_t));
            if (!tmp) abort();
            /* zero the new part of the array */
            if (tmp != seen_ts) {
                memset(tmp + seen_cap, 0, (new_seen_cap - seen_cap) * sizeof(uint64_t));
            }
            seen_ts = tmp;
            seen_cap = new_seen_cap;
        }

        /* Initialise first term if not yet done */
        if (max_computed == 0) {
            a_vals[1] = (seq_t)1;      /* a(1) = 1 by definition */
            max_computed = 1;
        }

        /* Compute missing terms up to n */
        for (idx_t m = max_computed + 1; m <= n; ++m) {
            seq_t sn = (seq_t)m;   /* n as seq_t for bit operations */
            uint64_t seen_count = 0;

            cur_ts++;  /* new timestamp for this m */
            if (cur_ts == 0) {   /* unlikely but guard against wrap‑around */
                cur_ts = 1;
                memset(seen_ts, 0, seen_cap * sizeof(uint64_t));
            }

            for (idx_t k = 1; k < m; ++k) {
                if ((a_vals[k] & sn) != (seq_t)0) {
                    size_t val = (size_t)a_vals[k];
                    if (seen_ts[val] != cur_ts) {
                        seen_ts[val] = cur_ts;
                        seen_count++;
                    }
                }
            }
            a_vals[m] = (seq_t)(1 + seen_count);
        }
        max_computed = n;
    }
    return a_vals[n];
}
