/*
 * A281702  Numbers k such that A001221(k) = A001221(A000326(k)).
 * https://oeis.org/A281702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A281702:
   Numbers k such that A001221(k) = A001221(A000326(k)).
   The algorithm simply tests consecutive integers until the required
   count of qualifying values has been reached, memoising the found
   terms so that calls in any order are fast. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/count_distinct_primes.h"

/* Helper: test whether a particular k satisfies the property. */
static inline int valid_k(uint64_t k)
{
    uint64_t pent;
    /* Compute k*(3*k-1)/2 without overflow for the relevant range. */
    if (k & 1) {
        uint64_t m = 3 * k - 1;          /* 3*k-1 is even when k is odd */
        pent = k * (m / 2);
    } else {
        uint64_t m = 3 * k - 1;          /* 3*k-1 is odd when k is even   */
        pent = (k / 2) * m;
    }
    return count_distinct_primes(k) == count_distinct_primes(pent);
}

/* Return the nth term of the sequence (indexing starts at 1). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        /* Undefined for n = 0; return 0 as a safe default. */
        return (seq_t)0;
    }

    /* ---------- Lazy memoisation of found terms ---------- */
    static seq_t *values = NULL;      /* stored sequence values */
    static size_t len    = 0;         /* number of stored values   */
    static size_t cap    = 0;         /* current allocation size   */
    static uint64_t last_checked = 0; /* largest integer already tested */

    /* Initialise storage on first call. */
    if (!values) {
        cap = 128;
        values = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Extend pre‑computation until we have at least n values. */
    while (len < (size_t)n) {
        /* Find the next valid k. */
        for (;;) {
            ++last_checked;               /* candidate k = last_checked */
            if (valid_k(last_checked)) {
                /* Ensure we have enough space in the array. */
                if (len == cap) {
                    cap *= 2;
                    values = (seq_t *)realloc(values, cap * sizeof(seq_t));
                }
                values[len++] = (seq_t)last_checked;
                break;  /* ready for the next required term   */
            }
        }
    }

    return values[n - 1];
}
