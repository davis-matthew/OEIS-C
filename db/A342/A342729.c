/*
 * A342729  Self numbers in base i-1: numbers not of the form k + A066323(k).
 * https://oeis.org/A342729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9832, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sumdigits_neg4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Self numbers in base -4 (A342729) – computed by sieving numbers k,
   marking g = k + digit_sum_neg4(k), and storing the non‑marked values
   as the self numbers.  The function expands the search limit until
   it contains at least the required index. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/sumdigits_neg4.h"

/* Static cache of self numbers and parameters for incremental growth. */
static seq_t *self_nums = NULL;   /* array of self numbers (0‑based index) */
static size_t self_count = 0;     /* number of self numbers stored   */
static idx_t current_limit = 0;   /* largest k processed so far      */

/* Rebuild the cache up to the specified limit. */
static void rebuild(idx_t limit)
{
    size_t len = (size_t)limit + 1;
    bool *marked = (bool *)calloc(len, sizeof(bool));
    if (!marked)
        abort();

    for (idx_t k = 1; k <= limit; ++k) {
        seq_t d = sumdigits_neg4(k);
        seq_t g = (seq_t)k + d;
        if ((idx_t)g <= limit)
            marked[(size_t)g] = true;
    }

    size_t est = limit / 2 + 10;            /* rough density estimate */
    seq_t *buf = (seq_t *)malloc(est * sizeof(seq_t));
    if (!buf)
        abort();

    size_t pos = 0;
    for (idx_t x = 1; x <= limit; ++x) {
        if (!marked[(size_t)x]) {
            if (pos >= est) {               /* grow buffer if needed */
                est <<= 1;
                buf = (seq_t *)realloc(buf, est * sizeof(seq_t));
                if (!buf)
                    abort();
            }
            buf[pos++] = (seq_t)x;
        }
    }

    free(marked);
    free(self_nums);
    self_nums = buf;
    self_count = pos;
    current_limit = limit;
}

/* Public sequence function.  a(1) is the first term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)               /* out of bounds – return 0 as a sentinel */
        return 0;

    if (self_count < (size_t)n) {
        idx_t limit = current_limit ? current_limit : 2000;
        while (self_count < (size_t)n) {
            limit <<= 1;          /* double the limit until enough */
            rebuild(limit);
        }
    }
    return self_nums[(size_t)(n - 1)];
}
