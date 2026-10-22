/*
 * A325597  a(n) is the least number not 2a(m) + a(m-1) for any m < n.
 * https://oeis.org/A325597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the least positive integer that is not representable as
 *          2*a(m)+a(m-1) for any 1 ≤ m < n.
 * The algorithm builds the sequence incrementally, keeping a boolean
 * table of forbidden values.  The table is expanded as new forbidden
 * values appear.
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache for the computed sequence values. */
    static seq_t *values      = NULL;
    static size_t values_cap  = 0;
    static size_t values_len  = 0;   /* number of terms stored (values[1..len]) */

    /* Static table of forbidden numbers of the form 2*a(m)+a(m-1). */
    static bool *blocked      = NULL;
    static size_t blocked_cap = 0;

    /* Next candidate to test for inclusion in the sequence. */
    static seq_t candidate = 1;

    /* Allocate initial buffers on first call. */
    if (!values) {
        values_cap = 4;
        values = (seq_t *)calloc(values_cap + 1, sizeof(seq_t));
        if (!values) abort();
    }
    if (!blocked) {
        blocked_cap = 4;
        blocked = (bool *)calloc(blocked_cap + 1, sizeof(bool));
        if (!blocked) abort();
    }

    /* Ensure 'values' can hold up to index 'n'. */
    if (n > values_cap) {
        size_t new_cap = values_cap ? values_cap : 4;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(values, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();
        values = tmp;
        values_cap = new_cap;
    }

    /* Generate terms until we have computed a(n). */
    while (values_len < n) {
        /* Find the next free candidate (not blocked). */
        while (true) {
            size_t ci = (size_t)candidate;
            if (ci > blocked_cap) break;        /* no known block beyond this point */
            if (!blocked[ci]) break;            /* candidate is free */
            ++candidate;
        }

        /* Store the candidate as the next sequence term. */
        ++values_len;
        values[values_len] = candidate;

        /* If we have at least two terms, compute the new forbidden value. */
        if (values_len >= 2) {
            seq_t forbid = 2 * values[values_len] + values[values_len - 1];
            size_t fi = (size_t)forbid;

            /* Expand the blocked table if necessary. */
            if (fi > blocked_cap) {
                size_t new_cap = blocked_cap ? blocked_cap : 4;
                while (new_cap < fi) new_cap <<= 1;
                bool *tmp = (bool *)realloc(blocked, (new_cap + 1) * sizeof(bool));
                if (!tmp) abort();
                /* Zero the newly allocated portion. */
                memset(tmp + blocked_cap + 1, 0, (new_cap - blocked_cap) * sizeof(bool));
                blocked = tmp;
                blocked_cap = new_cap;
            }

            blocked[fi] = true;
        }

        /* Advance candidate for the next iteration. */
        ++candidate;
    }

    return values[n];
}
