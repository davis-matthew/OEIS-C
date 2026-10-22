/*
 * A348054  Positive integers that are the product of two integers ending with
 *          7.
 * https://oeis.org/A348054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes OEIS A348054: positive integers that are the product of two integers ending with 7.
 * The algorithm searches successive numbers ending with 9 and checks whether they have a divisor
 * ending with 7 whose co‑factor also ends with 7.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Cache for the generated sequence */
static seq_t *seq = NULL;       /* array of computed terms */
static size_t capacity = 0;     /* allocated size of the array */
static idx_t length = 0;        /* number of terms stored so far */
static idx_t candidate_start = 49;   /* next number ending with 9 to test */

/* Return true iff m can be written as (7 + 10k) * (7 + 10l) */
static bool is_7product(seq_t m)
{
    seq_t lim = floor_sqrt(m);
    for (seq_t d = (seq_t)7; d <= lim; d = d + (seq_t)10) {
        if (m % d == (seq_t)0) {
            seq_t other = m / d;
            if (other % (seq_t)10 == (seq_t)7)
                return true;
        }
    }
    return false;
}

/* Ensure the cache can hold at least 'needed' elements */
static void grow_if_needed(idx_t needed)
{
    if ((size_t)needed <= capacity)
        return;
    size_t new_cap = capacity ? capacity * 2 : 4;
    while ((size_t)needed > new_cap)
        new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    capacity = new_cap;
}

/* Return the nth term (n >= 1) of the sequence */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset 1; return 0 as a safe sentinel */
        return (seq_t)0;

    /* Generate terms up to n if necessary */
    if (length < n) {
        idx_t m = candidate_start;
        while (length < n) {
            seq_t val = (seq_t)m;
            if (is_7product(val)) {
                grow_if_needed((idx_t)(length + 1));
                seq[length++] = val;
            }
            m += 10;    /* next number ending with 9 */
        }
        candidate_start = m;
    }
    return seq[n - 1];
}
