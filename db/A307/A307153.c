/*
 * A307153  Sequence gives pair of terms giving the numbers of previous even
 *          digits and previous odd digits; a(0)=0.
 * https://oeis.org/A307153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 203, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive definition: a(0)=0.  
   For k ≥ 0:
        a(2k+1) = total number of even decimal digits in a(0)…a(2k);
        a(2k+2) = total number of odd  decimal digits in a(0)…a(2k+1); */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_counts.h"

static seq_t *seq = NULL;          /* array of computed values */
static size_t seq_cap = 0;         /* allocated capacity of seq */
static idx_t  max_computed = (idx_t)-1; /* highest n that has been computed (-1 = none) */
static seq_t evenCount = 0;        /* running count of even decimal digits seen */
static seq_t oddCount  = 0;        /* running count of odd  decimal digits seen */
static int   initialized = 0;      /* flag that a(0) has been initialised */

/* enlarge the storage so that it can hold indices up to n */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq = tmp;
    seq_cap = new_cap;
}

/* add all decimal digits of x to evenCount / oddCount */
static void add_digits(seq_t x)
{
    unsigned char freq[10];
    digit_counts(x, freq);
    evenCount += (seq_t)(freq[0] + freq[2] + freq[4] + freq[6] + freq[8]);
    oddCount  += (seq_t)(freq[1] + freq[3] + freq[5] + freq[7] + freq[9]);
}

seq_t a(idx_t n)
{
    /* initialise a(0) once */
    if (!initialized)
    {
        ensure_capacity(0);
        seq[0] = (seq_t)0;
        evenCount = 0;
        oddCount  = 0;
        add_digits(seq[0]);          /* account for the digits of a(0) */
        max_computed = 0;
        initialized = 1;
    }

    /* if already computed, return cached value */
    if (n <= max_computed)
        return seq[(size_t)n];

    /* compute all missing terms up to n */
    ensure_capacity(n);
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        seq_t val = (i & 1ULL) ? evenCount : oddCount; /* odd → evenCount, even → oddCount */
        seq[(size_t)i] = val;
        add_digits(val);
    }
    max_computed = n;
    return seq[(size_t)n];
}
