/*
 * A275708  a(1)=1, a(2)=2; thereafter a(n) is the smallest positive integer
 *          not yet used such that a(n)+a(n-1)-a(n-2) is a square.
 * https://oeis.org/A275708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   Definition of OEIS sequence A275708.
   a(1)=1, a(2)=2; thereafter a(n) is the smallest positive integer not yet used
   such that a(n)+a(n-1)-a(n-2) is a perfect square.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t x) returns ⌊√x⌋ */

/* Static storage for the sequence and the used‑number table */
static seq_t *sequence = NULL;
static size_t seq_cap   = 0;           /* allocated capacity for sequence   */
static size_t seq_len   = 0;           /* number of terms stored (>=0)      */

static bool *used = NULL;              /* used[i]==true iff i already used */
static size_t used_cap = 0;            /* allocated capacity for used table */

/* Ensure that 'used' is large enough to index up to 'max' (inclusive). */
static void ensure_used_capacity(size_t max)
{
    if (max < used_cap)
        return;
    size_t new_cap = used_cap ? used_cap * 2 : 1024;
    while (new_cap <= max)
        new_cap *= 2;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof *tmp);
    if (!tmp)
        abort();                    /* out of memory */
    /* initialise new entries to false */
    for (size_t i = used_cap; i < new_cap; ++i)
        tmp[i] = false;
    used = tmp;
    used_cap = new_cap;
}

/* Ensure that 'sequence' can hold at least 'min_cap' terms. */
static void ensure_seq_capacity(size_t min_cap)
{
    if (min_cap <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < min_cap)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(sequence, new_cap * sizeof *tmp);
    if (!tmp)
        abort();                    /* out of memory */
    sequence = tmp;
    seq_cap = new_cap;
}

/* Compute the next term a(k) where k = seq_len + 1. */
static seq_t compute_next_term(void)
{
    /* previous two terms */
    seq_t prev1 = sequence[seq_len - 1];  /* a(k-1) */
    seq_t prev2 = sequence[seq_len - 2];  /* a(k-2) */

    seq_t candidate = 1;
    for (;; ++candidate) {
        /* enlarge 'used' if necessary */
        ensure_used_capacity((size_t)candidate);
        if (used[(size_t)candidate])
            continue;                 /* already used */

        /* D = candidate + prev1 - prev2 */
        /* Guard against unsigned underflow: skip if would be negative. */
        if (prev2 > candidate + prev1)
            continue;

        seq_t D = candidate + prev1 - prev2;
        if (D == 0)
            continue;                 /* 0 is not a square in this context */

        seq_t s = floor_sqrt(D);
        if (s * s == D) {
            /* valid candidate */
            used[(size_t)candidate] = true;
            return candidate;
        }
    }
}

/* The exported function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n=0, guard against misuse */

    /* make sure the array is large enough */
    ensure_seq_capacity((size_t)n);

    /* initialise first two terms if they are missing */
    if (seq_len == 0) {
        sequence[0] = 1;
        ensure_used_capacity(1);
        used[1] = true;
        seq_len = 1;
    }
    if (seq_len == 1) {
        sequence[1] = 2;
        ensure_used_capacity(2);
        used[2] = true;
        seq_len = 2;
    }

    /* generate terms until we reach the requested index */
    while (seq_len < (size_t)n) {
        seq_t val = compute_next_term();
        sequence[seq_len++] = val;
    }

    return sequence[n - 1];
}
