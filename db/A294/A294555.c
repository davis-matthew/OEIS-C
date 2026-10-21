/*
 * A294555  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) + b(n-2) + 3, where a(0) = 1, a(1) = 2, b(0) = 3, and
 *          (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A294555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-1) + b(n-2) + 3, a(0) = 1, a(1) =
 *            2, b(0) = 3, b(1) = 4 (least new numbers)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static tables for a(n) and b(n) */
static seq_t *a_arr = NULL;
static size_t a_cap = 0;
static size_t a_len = 0;

static seq_t *b_arr = NULL;
static size_t b_cap = 0;
static size_t b_len = 0;

/* Next candidate for b, starting at 5 after b(1)=4 */
static seq_t next_candidate = (seq_t)5;

/* Initialize the base terms once */
static void init_sequences(void)
{
    if (a_arr != NULL)
        return;

    a_cap = 4;
    a_arr = (seq_t *)malloc(a_cap * sizeof(seq_t));
    a_arr[0] = (seq_t)1;
    a_arr[1] = (seq_t)2;
    a_len = 2;

    b_cap = 4;
    b_arr = (seq_t *)malloc(b_cap * sizeof(seq_t));
    b_arr[0] = (seq_t)3;
    b_arr[1] = (seq_t)4;
    b_len = 2;

    next_candidate = (seq_t)5;
}

/* Binary search to test if x is already an a value */
static bool seq_contains_a(seq_t x)
{
    size_t lo = 0, hi = a_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (a_arr[mid] == x)
            return true;
        if (a_arr[mid] < x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* Ensure that a(n) is computed up to index n */
static void ensure_a(idx_t n)
{
    init_sequences();

    while (a_len <= n) {
        /* Generate required b indices */
        while (b_len <= a_len - 1) {
            seq_t cand = next_candidate;
            while (seq_contains_a(cand))
                cand++;

            /* grow b array if needed */
            if (b_len >= b_cap) {
                b_cap = b_cap ? b_cap * 2 : 4;
                b_arr = (seq_t *)realloc(b_arr, b_cap * sizeof(seq_t));
            }
            b_arr[b_len++] = cand;
            next_candidate = cand + (seq_t)1;
        }

        /* Compute a(current index) */
        seq_t val = a_arr[a_len - 1] + a_arr[a_len - 2]
                   + b_arr[a_len - 1] + b_arr[a_len - 2] + (seq_t)3;

        /* grow a array if needed */
        if (a_len >= a_cap) {
            a_cap = a_cap ? a_cap * 2 : 4;
            a_arr = (seq_t *)realloc(a_arr, a_cap * sizeof(seq_t));
        }
        a_arr[a_len++] = val;
    }
}

seq_t a(idx_t n)
{
    init_sequences();
    if (n < a_len)
        return a_arr[(size_t)n];
    ensure_a(n);
    return a_arr[(size_t)n];
}
