/*
 * A389532  Standard composition numbers of integer compositions with maximum
 *          = length.
 * https://oeis.org/A389532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer k whose standard composition (A066099)
   has maximum part equal to its length. */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of found terms   */
static idx_t  cached = 0;            /* number of terms cached*/
static idx_t  capacity = 0;          /* current capacity      */
static idx_t  next_candidate = 1;    /* next k to test        */

/* Test whether the composition of k satisfies max(part) == length. */
static int ok(idx_t k)
{
    idx_t pos   = 0;   /* last position that had a 1 (1‑based)     */
    idx_t max    = 0;  /* largest part found so far                */
    idx_t length = 0;  /* number of parts                         */
    idx_t bit    = 1;  /* current position (1‑based)              */

    while (k != 0) {
        if (k & 1ULL) {
            idx_t part = bit - pos;
            if (part > max) max = part;
            length++;
            pos = bit;
        }
        k >>= 1ULL;
        bit++;
    }
    return max == length;
}

/* Generate terms until cache holds at least n elements. */
static void extend_cache(idx_t n)
{
    /* initial allocation */
    if (capacity == 0) {
        capacity = 128;
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
    }

    while (cached < n) {
        /* find next valid k */
        while (!ok(next_candidate))
            next_candidate++;

        /* store it */
        if (cached >= capacity) {
            capacity *= 2;
            seq_t *tmp = (seq_t*)realloc(cache, capacity * sizeof(seq_t));
            if (!tmp) abort();          /* memory allocation failed */
            cache = tmp;
        }
        cache[cached++] = (seq_t)next_candidate;
        next_candidate++;               /* start next search after this k */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1 */
    if (cached < n) extend_cache(n);
    return cache[n - 1];
}
