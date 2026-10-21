/*
 * A295618  Solution of the complementary equation a(n) = 2*a(n-1) - a(n-3) +
 *          b(n-2), where a(0) = 2, a(1) = 4, a(2) = 6, b(0) = 1, b(1) = 3,
 *          b(2) = 5, and (a(n)) and (b(n)) are increasing complementary
 *          sequences.
 * https://oeis.org/A295618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A295618
   a(n) = 2*a(n-1) - a(n-3) + b(n-2)
   a(0)=2, a(1)=4, a(2)=6
   b(0)=1, b(1)=3, b(2)=5
   (a(n)) and (b(n)) are increasing complementary sequences. */
#include <stdlib.h>
#include <stdbool.h>

/* Static state for memoisation ---------------------------------------- */
static seq_t *a_seq = NULL;
static seq_t *b_seq = NULL;
static idx_t a_len = 0;
static idx_t b_len = 0;
static size_t a_cap = 0;
static size_t b_cap = 0;
static seq_t next_candidate = 0;
static bool initialized = false;

/* Return true if val is an element of a_seq[0..up_to] */
static bool is_in_a(seq_t val, idx_t up_to)
{
    size_t lo = 0;
    size_t hi = (size_t)up_to;
    while (lo <= hi) {
        size_t mid = lo + (hi - lo) / 2;
        seq_t mid_val = a_seq[mid];
        if (val == mid_val)
            return true;
        if (val < mid_val) {
            if (mid == 0)
                break;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return false;
}

/* Main sequence function ---------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise on the first call */
    if (!initialized) {
        a_cap = 4;
        a_seq = (seq_t *)malloc(a_cap * sizeof(seq_t));
        b_cap = 4;
        b_seq = (seq_t *)malloc(b_cap * sizeof(seq_t));

        a_seq[0] = 2; a_seq[1] = 4; a_seq[2] = 6;
        b_seq[0] = 1; b_seq[1] = 3; b_seq[2] = 5;
        a_len = 3;   b_len = 3;

        next_candidate = 7;
        initialized = true;
    }

    /* grow the sequences if n is beyond the currently computed range */
    if (n >= a_len) {
        /* ensure capacity for a_seq */
        if (a_cap <= (size_t)n) {
            size_t new_cap = a_cap ? a_cap : 1;
            while (new_cap <= (size_t)n)
                new_cap <<= 1;
            a_seq = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
            a_cap = new_cap;
        }
        /* ensure capacity for b_seq */
        if (b_cap <= (size_t)n) {
            size_t new_cap = b_cap ? b_cap : 1;
            while (new_cap <= (size_t)n)
                new_cap <<= 1;
            b_seq = (seq_t *)realloc(b_seq, new_cap * sizeof(seq_t));
            b_cap = new_cap;
        }

        /* compute all missing terms up to n */
        for (idx_t k = a_len; k <= n; ++k) {
            if (k >= 3) {
                seq_t val = 2 * a_seq[k - 1] - a_seq[k - 3] + b_seq[k - 2];
                a_seq[k] = val;
            }
            /* compute b[k] : smallest integer not in a_seq[0..k]  */
            while (is_in_a(next_candidate, k)) {
                ++next_candidate;
            }
            b_seq[k] = next_candidate;
            ++next_candidate;
        }
        a_len = n + 1;
        b_len = n + 1;
    }

    return a_seq[n];
}
