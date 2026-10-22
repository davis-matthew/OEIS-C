/*
 * A378293  a(1) = 1. For n > 1 if a(n-1) is a novel term a(n) = number of
 *          a(i); 1 <= i <= n-1 whose sum of decimal digits does not exceed
 *          A007953(a(n-1)). If a(n-1) has occurred k (>1) times, a(n) =
 *          k*a(n-1).
 * https://oeis.org/A378293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 6685, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A378293 computed by recurrence with memoization.
 * a(1) = 1.
 * For n > 1, let x = a(n-1) and t = digit_sum(x).
 * If x has appeared before (k = # {i < n-1 : a(i) = x}), then
 *     a(n) = (k + 1) * x.
 * Else (x is novel), then
 *     a(n) = # {i <= n-1 : digit_sum(a(i)) <= t}.
 *
 * The implementation stores all generated terms and their digit sums
 * and recomputes the necessary counts in O(n) per term.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"   /* seq_t digit_sum(seq_t v); */
#include "../utils/mul_small.h"   /* seq_t mul_small(seq_t x, unsigned int m); */

/* static state for memoization */
static seq_t *terms = NULL;      /* array of generated terms          */
static int   *dsums = NULL;      /* corresponding digit sums          */
static size_t capacity = 0;      /* current allocated size            */
static size_t length   = 0;      /* number of terms stored (len >= 1) */

/* Helper to expand storage */
static void ensure_capacity(size_t needed) {
    if (needed > capacity) {
        size_t new_cap = capacity ? capacity * 2 : 128;
        if (new_cap < needed) new_cap = needed;
        terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        dsums = (int*)realloc(dsums, new_cap * sizeof(int));
        capacity = new_cap;
    }
}

/* Return the nth term of sequence A378293 */
seq_t a(idx_t n) {
    /* n is at least 1, otherwise return 0 */
    if (n == 0) return (seq_t)0;

    /* lazily initialize with the first term */
    if (length == 0) {
        ensure_capacity(1);
        terms[0] = (seq_t)1;
        dsums[0] = 1;          /* digit sum of 1 */
        length = 1;
    }

    /* generate terms up to index n if needed */
    while (length < (size_t)n) {
        seq_t prev = terms[length - 1];
        int  prev_dsum = dsums[length - 1];

        /* count how many times prev occurred before the current position */
        size_t count_before = 0;
        for (size_t i = 0; i + 1 < length; ++i) {
            if (terms[i] == prev) ++count_before;
        }

        seq_t new_term;
        if (count_before == 0) {           /* novel term */
            idx_t threshold = (idx_t)digit_sum(prev);
            size_t count_leq = 0;
            for (size_t i = 0; i < length; ++i) {
                if ((idx_t)dsums[i] <= threshold) ++count_leq;
            }
            new_term = (seq_t)count_leq;
        } else {                           /* repeated term */
            size_t k = count_before + 1;
            new_term = mul_small(prev, (unsigned int)k);
        }

        /* store the new term */
        ensure_capacity(length + 1);
        terms[length] = new_term;
        dsums[length] = (int)digit_sum(new_term);
        ++length;
    }

    return terms[(size_t)n - 1];
}
