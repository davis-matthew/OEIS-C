/*
 * A295144  Solution of the complementary equation a(n) = 2*a(n-1) + a(n-2) +
 *          b(n-1), where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4, and (a(n))
 *          and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + b(n-1), with a(0)=1, a(1)=3, b(0)=2,
 *            b(1)=4. Complementary increasing sequences
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static storage for the sequences and auxiliary data */
static seq_t *a_seq   = NULL;
static size_t   a_cnt = 0, a_cap = 0;
static seq_t *b_seq   = NULL;
static size_t   b_cnt = 0, b_cap = 0;
static idx_t    next_missing = 0;
static bool     initialized = false;

/* simple binary search in a sorted array */
static bool binary_search(const seq_t *arr, size_t len, seq_t val)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid] < val) {
            lo = mid + 1;
        } else if (arr[mid] > val) {
            hi = mid;
        } else {
            return true;
        }
    }
    return false;
}

/* ensure capacity of an array */
static void ensure_a_capacity(size_t needed)
{
    if (needed <= a_cap) return;
    a_cap = (a_cap > 0 ? a_cap * 2 : 16);
    while (a_cap < needed) a_cap *= 2;
    a_seq = (seq_t *)realloc(a_seq, a_cap * sizeof(seq_t));
}

static void ensure_b_capacity(size_t needed)
{
    if (needed <= b_cap) return;
    b_cap = (b_cap > 0 ? b_cap * 2 : 16);
    while (b_cap < needed) b_cap *= 2;
    b_seq = (seq_t *)realloc(b_seq, b_cap * sizeof(seq_t));
}

/* initialize the base values of the sequences */
static void init_sequences(void)
{
    a_cap = 16;
    a_seq = (seq_t *)malloc(a_cap * sizeof(seq_t));
    b_cap = 16;
    b_seq = (seq_t *)malloc(b_cap * sizeof(seq_t));

    /* base values */
    a_seq[0] = (seq_t)1;
    a_seq[1] = (seq_t)3;
    a_cnt   = 2;

    b_seq[0] = (seq_t)2;
    b_seq[1] = (seq_t)4;
    b_cnt   = 2;

    next_missing = 5;   /* first candidate not used yet */
    initialized = true;
}

/* the required function */
seq_t a(idx_t n)
{
    if (!initialized) init_sequences();

    size_t idx = (size_t)n;
    if (idx < a_cnt) return a_seq[idx];

    /* extend the sequences up to the requested index */
    while (a_cnt <= idx) {
        /* ensure b_seq has entry for a_cnt-1 */
        while (b_cnt <= a_cnt - 1) {
            seq_t cand = (seq_t)next_missing;
            /* find the next missing integer */
            while (binary_search(a_seq, a_cnt, cand) ||
                   binary_search(b_seq, b_cnt, cand)) {
                ++next_missing;
                cand = (seq_t)next_missing;
            }
            ensure_b_capacity(b_cnt + 1);
            b_seq[b_cnt++] = cand;
            ++next_missing;  /* next candidate after the one we just used */
        }

        /* compute a[a_cnt] using the recurrence */
        seq_t bi_minus1 = b_seq[a_cnt - 1];
        seq_t a_new = (seq_t)2 * a_seq[a_cnt - 1] + a_seq[a_cnt - 2] + bi_minus1;

        /* store the new a value */
        ensure_a_capacity(a_cnt + 1);
        a_seq[a_cnt++] = a_new;
    }

    return a_seq[idx];
}
