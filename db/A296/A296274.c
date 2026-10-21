/*
 * A296274  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1)*b(n), where a(0) = 1, a(1) = 4, b(0) = 2, b(1) = 3, b(2) =
 *          5, and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 38, u64 n <= 85, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296274:
   a(n) = a(n-1) + a(n-2) + b(n-1)*b(n)
   a(0)=1, a(1)=4
   b(0)=2, b(1)=3, b(2)=5
   a() and b() are complementary increasing sequences.
*/

#include <stddef.h>
#include <stdlib.h>

/* Static storage for the sequences and their meta‑data. */
static seq_t *a_arr  = NULL;
static idx_t  a_len  = 0, a_cap  = 0;
static seq_t *b_arr  = NULL;
static idx_t  b_len  = 0, b_cap  = 0;
static seq_t  next_candidate = 0;   /* next integer to try as a b value   */
static idx_t  next_a_idx     = 0;   /* a index used during b generation   */

/* Ensure the array for a has space for at least 'needed' elements. */
static void ensure_a_cap(idx_t needed) {
    if (a_cap < needed) {
        size_t new_cap = a_cap ? a_cap * 2 : 4;
        if (new_cap < (size_t)needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        a_arr = tmp;
        a_cap = new_cap;
    }
}

/* Ensure the array for b has space for at least 'needed' elements. */
static void ensure_b_cap(idx_t needed) {
    if (b_cap < needed) {
        size_t new_cap = b_cap ? b_cap * 2 : 4;
        if (new_cap < (size_t)needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(b_arr, new_cap * sizeof(seq_t));
        b_arr = tmp;
        b_cap = new_cap;
    }
}

/* Generate the next term of the complementary sequence b. */
static void generate_next_b(void) {
    while (1) {
        /* Advance next_a_idx so that a_arr[next_a_idx] >= next_candidate. */
        while (next_a_idx < a_len && a_arr[next_a_idx] < next_candidate) {
            ++next_a_idx;
        }
        /* If the candidate is in a, skip it. */
        if (next_a_idx < a_len && a_arr[next_a_idx] == next_candidate) {
            ++next_candidate;
            continue;
        }
        /* Candidate is not in a – it is the next b. */
        ensure_b_cap(b_len + 1);
        b_arr[b_len] = next_candidate;
        ++b_len;
        ++next_candidate;   /* next search starts after the new b */
        break;
    }
}

/* Lazy initialization of the sequences. */
static void init_sequences(void) {
    if (a_cap != 0) return;   /* already initialised */

    /* Allocate initial storage. */
    a_cap = 8;
    a_arr = (seq_t *)malloc(a_cap * sizeof(seq_t));
    b_cap = 8;
    b_arr = (seq_t *)malloc(b_cap * sizeof(seq_t));

    /* Initial values of a and b. */
    a_arr[0] = (seq_t)1;
    a_arr[1] = (seq_t)4;
    a_len  = 2;

    b_arr[0] = (seq_t)2;
    b_arr[1] = (seq_t)3;
    b_arr[2] = (seq_t)5;
    b_len  = 3;

    /* Compute a(2) using the recurrence. */
    ensure_a_cap(3);
    a_arr[2] = a_arr[1] + a_arr[0] + b_arr[1] * b_arr[2];
    a_len  = 3;

    /* Set the starting point for searching the next b. */
    next_candidate = b_arr[2] + (seq_t)1;   /* 6 */
    next_a_idx = 0;
}

/* Public function required by the judge. */
seq_t a(idx_t n) {
    init_sequences();

    if (n < a_len)
        return a_arr[n];

    for (idx_t i = a_len; i <= n; ++i) {
        /* Ensure we have b[i] available. */
        while (b_len <= i) generate_next_b();

        seq_t a_im1 = a_arr[i - 1];
        seq_t a_im2 = a_arr[i - 2];
        seq_t b_im1 = b_arr[i - 1];
        seq_t b_i   = b_arr[i];

        seq_t value = a_im1 + a_im2 + b_im1 * b_i;

        ensure_a_cap(i + 1);
        a_arr[i] = value;
        a_len = i + 1;
    }

    return a_arr[n];
}
