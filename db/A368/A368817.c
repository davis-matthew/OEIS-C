/*
 * A368817  Sum of the refactorable numbers less than n that do not divide n.
 * https://oeis.org/A368817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum of refactorable numbers < n that do not divide n
 * Range:     u8 n <= 60, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global tables for divisor counts and refactorability flags. */
static idx_t   maxN   = 0;           /* Largest n processed so far (inclusive) */
static uint32_t *divcnt = NULL;     /* divisor counts */
static uint8_t  *is_ref = NULL;     /* 1 if i is refactorable, else 0 */

/* Ensure that tables are computed up to 'limit' (inclusive). */
static void ensure_tables(idx_t limit)
{
    if (limit <= maxN)
        return;                     /* already up to date */

    /* Grow the arrays to hold indices 0..limit */
    if (divcnt)
        divcnt = (uint32_t *)realloc(divcnt, (limit + 1) * sizeof(uint32_t));
    else
        divcnt = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (is_ref)
        is_ref = (uint8_t *)realloc(is_ref, (limit + 1) * sizeof(uint8_t));
    else
        is_ref = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));

    /* Initialise the new entries to zero. */
    for (idx_t i = maxN + 1; i <= limit; ++i) {
        divcnt[i] = 0;
        is_ref[i] = 0;
    }

    /* Sieve divisor counts for 1..limit. */
    for (idx_t i = 1; i <= limit; ++i) {
        for (idx_t j = i; j <= limit; j += i) {
            ++divcnt[j];
        }
    }

    /* Mark refactorable numbers. */
    for (idx_t i = 1; i <= limit; ++i) {
        if (divcnt[i] != 0 && (i % divcnt[i] == 0))
            is_ref[i] = 1;
    }

    maxN = limit;
}

/* -------------------------------------------------------------- */
/*  Sequence entry a(n)                                          */
/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 0 by definition */
        return 0;

    /* Need divisor data up to n-1. */
    ensure_tables(n - 1);

    seq_t sum = 0;
    for (idx_t k = 1; k < n; ++k) {
        if (is_ref[k] && (n % k != 0))
            sum += (seq_t)k;
    }
    return sum;
}
