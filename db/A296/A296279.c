/*
 * A296279  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2)*b(n-1)*b(n), where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4,
 *          b(2) = 5, and (a(n)) and (b(n)) are increasing complementary
 *          sequences.
 * https://oeis.org/A296279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-2)*b(n-1)*b(n), a(0)=1, a(1)=3;
 *            b(0)=2, b(1)=4, b(2)=5 and a, b are complementary
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 35, u64 n <= 81, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Global caches for the sequences; they are allocated lazily. */
static seq_t *a_cache = NULL;
static size_t a_len = 0, a_cap = 0;
static seq_t *b_cache = NULL;
static size_t b_len = 0, b_cap = 0;

/* Grow a_cache to hold at least `need` elements. */
static void grow_a(size_t need)
{
    if (a_cap == 0) {
        a_cap = 8;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
    }
    while (a_cap < need) {
        a_cap *= 2;
        a_cache = (seq_t *)realloc(a_cache, a_cap * sizeof(seq_t));
    }
}

/* Grow b_cache to hold at least `need` elements. */
static void grow_b(size_t need)
{
    if (b_cap == 0) {
        b_cap = 8;
        b_cache = (seq_t *)malloc(b_cap * sizeof(seq_t));
    }
    while (b_cap < need) {
        b_cap *= 2;
        b_cache = (seq_t *)realloc(b_cache, b_cap * sizeof(seq_t));
    }
}

/* State used while generating the complementary b sequence. */
static size_t a_check_idx = 0;      /* next a index to consider when skipping b. */
static seq_t b_next_candidate = (seq_t)6;   /* next integer to test for b. */

/* Generate the next element of the complementary sequence b. */
static void generate_next_b(void)
{
    seq_t val = b_next_candidate;
    /* Skip any numbers that are already in a_cache. */
    while (a_check_idx < a_len && a_cache[a_check_idx] < val)
        ++a_check_idx;
    if (a_check_idx < a_len && a_cache[a_check_idx] == val) {
        /* Collision: the candidate is in a; skip it. */
        val = a_cache[a_check_idx] + (seq_t)1;
        /* Advance a_check_idx past the skipped value. */
        while (a_check_idx < a_len && a_cache[a_check_idx] < val)
            ++a_check_idx;
    }
    /* Store the new b value. */
    grow_b(b_len + 1);
    b_cache[b_len++] = val;
    b_next_candidate = val + (seq_t)1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Initialise on first call. */
    if (a_len == 0) {
        /* a(0)=1, a(1)=3. */
        grow_a(2);
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)3;
        a_len = 2;

        /* b(0)=2, b(1)=4, b(2)=5. */
        grow_b(3);
        b_cache[0] = (seq_t)2;
        b_cache[1] = (seq_t)4;
        b_cache[2] = (seq_t)5;
        b_len = 3;

        b_next_candidate = (seq_t)6;
        a_check_idx = 0;
    }

    /* If the value is already known, return it. */
    if ((idx_t)n < a_len)
        return a_cache[n];

    /* Extend the sequences up to index n. */
    for (size_t i = a_len; i <= (size_t)n; ++i) {
        /* Ensure the required b values are available. */
        for (size_t j = b_len; j <= i; ++j)
            generate_next_b();

        /* Compute a[i] using the recurrence. */
        seq_t prod = b_cache[i - 2];
        prod *= b_cache[i - 1];
        prod *= b_cache[i];
        seq_t val = a_cache[i - 1] + a_cache[i - 2] + prod;

        grow_a(a_len + 1);
        a_cache[a_len++] = val;
    }

    return a_cache[n];
}
