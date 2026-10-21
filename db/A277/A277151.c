/*
 * A277151  Lexicographically least sequence of nonnegative integers that
 *          avoids 11/6-powers.
 * https://oeis.org/A277151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9324, u16 n <= 9324, u32 n <= 9324, u64 n <= 9324,
 *            u128 n <= 9324, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Greedy construction of the lexicographically least sequence avoiding 11/6‑powers. */

#include <stdlib.h>
#include <stdbool.h>

/* Static storage for the generated sequence. */
static seq_t *seq    = NULL;   /* array of values          */
static size_t len    = 0;      /* current number of values */
static size_t cap    = 0;      /* allocated capacity       */

/* Ensure the dynamic array has room for at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= cap)
        return;
    size_t new_cap = cap ? cap << 1 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                 /* out of memory */
    seq = tmp;
    cap = new_cap;
}

/* Check whether the most recently added value creates an 11/6‑power
   ending at the new position.  new_len is the length of the sequence
   after the tentative insertion. */
static bool candidate_valid(size_t new_len)
{
    /* For each possible k (root length 6k) that fits in the
       current window, test whether the last 11k/6k elements form
       an 11/6‑power. */
    for (size_t k = 1; 11 * k <= new_len; ++k) {
        size_t start = new_len - 11 * k;   /* start of the 11k block */
        size_t shift = 6 * k;              /* offset to the suffix */
        size_t limit = 5 * k;              /* number of elements to compare */
        bool power = true;
        for (size_t i = 0; i < limit; ++i) {
            if (seq[start + shift + i] != seq[start + i]) {
                power = false;
                break;
            }
        }
        if (power) /* full match ⇒ forbidden pattern */
            return false;
    }
    return true; /* candidate is acceptable */
}

/* Greedy step: find and append the smallest nonnegative integer
   that keeps the sequence 11/6‑power free. */
static seq_t append_next(void)
{
    int cand = 0;
    for (;;) {
        /* Reserve space and tentatively insert the candidate. */
        if (len == cap)
            ensure_capacity(len + 1);
        seq[len] = (seq_t)cand;

        /* Test validity using the tentative new length (len+1). */
        if (candidate_valid(len + 1)) {
            ++len;                    /* actually append */
            return seq[len - 1];
        }
        /* otherwise try next integer */
        ++cand;
    }
}

/* Public access function required by the framework. */
seq_t a(idx_t n)
{
    if ((size_t)n >= len) {
        for (idx_t i = (idx_t)len; i <= n; ++i)
            (void)append_next();
    }
    return seq[(size_t)n];
}
