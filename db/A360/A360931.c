/*
 * A360931  a(1) = 2, a(2) = 3; for n > 2, a(n) is the smallest number greater
 *          than 1 that has not appeared such that |a(n) - a(n-1)| has a
 *          common factor with a(n-2).
 * https://oeis.org/A360931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 2, a(2) = 3;
   for n > 2, a(n) is the smallest integer > 1 that has not yet
   appeared such that |a(n) - a(n-1)| shares a common factor with a(n-2). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Euclidean gcd on seq_t values. */
static inline seq_t my_gcd(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Static cache of the sequence. */
static seq_t *seq = NULL;          /* array holding a(1), a(2), ... */
static size_t seq_cap = 0;         /* allocated capacity of seq    */
static size_t seq_len = 0;         /* number of terms stored       */

/* Ensure the array can hold at least 'need' elements. */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap == 0 ? 4 : seq_cap * 2;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq = new_seq;
    seq_cap = new_cap;
}

/* Linear search to test whether a value has already appeared in seq. */
static bool used_in_seq(seq_t val)
{
    for (size_t i = 0; i < seq_len; ++i)
        if (seq[i] == val)
            return true;
    return false;
}

/* Sequence generator. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for non‑positive indices */

    /* Initialise the first two terms once. */
    if (seq_len == 0) {
        ensure_seq_capacity(4);
        seq[0] = 2;
        seq[1] = 3;
        seq_len = 2;
    }

    /* Generate terms up to the requested index. */
    while (seq_len < (size_t)n) {
        seq_t last        = seq[seq_len - 1];
        seq_t second_last = seq[seq_len - 2];
        seq_t cand = 2;                     /* start searching from 2 */
        /* Find the smallest unused candidate satisfying the gcd condition. */
        while (true) {
            if (used_in_seq(cand))
                { ++cand; continue; }

            seq_t diff = cand > last ? cand - last : last - cand;
            seq_t g    = my_gcd(diff, second_last);
            if (g > 1) {                      /* found the required term */
                ensure_seq_capacity(seq_len + 1);
                seq[seq_len] = cand;
                ++seq_len;
                break;
            }
            ++cand;
        }
    }

    return seq[n - 1];
}
