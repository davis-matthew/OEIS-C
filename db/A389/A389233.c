/*
 * A389233  a(0) = 0 and a(n) = a(n-1) - n if it is > 0 and not already in the
 *          sequence, otherwise a(n) = n*a(n-1) + 1.
 * https://oeis.org/A389233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 240, u32 n <= 3620, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0;
   a(n) = a(n-1) – n   if a(n-1) > n and the result is not already in the sequence,
   otherwise a(n) = n * a(n-1) + 1.
*/
#include <stdlib.h>

static seq_t *values = NULL;   /* sequence cache */
static size_t values_len = 0;  /* number of terms stored */
static size_t values_cap = 0;  /* allocated capacity */

/* Ensure the cache has space for at least 'need' elements. */
static void grow_cache(size_t need)
{
    if (values_cap < need) {
        size_t new_cap = values_cap ? values_cap << 1 : 8;
        if (new_cap < need) new_cap = need;
        values = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
        values_cap = new_cap;
    }
}

/* Check whether the value is already in the sequence. */
static int contains(seq_t x)
{
    for (size_t i = 0; i < values_len; ++i)
        if (values[i] == x) return 1;
    return 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Initialize on first call. */
    if (values_len == 0) {
        grow_cache(1);
        values[0] = (seq_t)0;
        values_len = 1;
    }

    /* Compute terms up to index 'n' if necessary. */
    if (values_len <= (size_t)n) {
        grow_cache((size_t)n + 1);
        for (idx_t i = (idx_t)values_len; i <= n; ++i) {
            seq_t prev = values[i - 1];
            seq_t next;

            if (prev > (seq_t)i) {          /* candidate does not underflow */
                seq_t cand = prev - (seq_t)i;
                if (!contains(cand))
                    next = cand;
                else
                    next = (seq_t)i * prev + (seq_t)1;
            } else {
                /* candidate <= 0 or underflows */
                next = (seq_t)i * prev + (seq_t)1;
            }

            values[i] = next;
        }
        values_len = (size_t)n + 1;
    }

    return values[n];
}
