/*
 * A303879  Lexicographically earliest sequence of distinct terms such that
 *          a(n) shares at least one odd digit with a(n+1).
 * https://oeis.org/A303879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 192, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   has_digit, odd_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Generates OEIS A303879 – the lexicographically earliest
   sequence of distinct terms such that each term shares at least one
   odd decimal digit with the next.  a(1)=1.  For n>1 the smallest
   unused integer that shares an odd digit with a(n‑1) is chosen. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_digit.h"
#include "../utils/odd_digits.h"

/* Global storage for the sequence and used numbers */
static seq_t *seq_arr  = NULL;   /* a(1), a(2), ... */
static size_t seq_len  = 0;      /* number of terms stored */
static size_t seq_cap  = 0;      /* capacity of seq_arr */

static seq_t *used_arr = NULL;   /* list of values already used */
static size_t used_len = 0;      /* number of used values */
static size_t used_cap = 0;      /* capacity of used_arr */

/* For each odd digit d (1,3,5,7,9) keep the smallest unused integer
   that contains d.  0 indicates that it has not yet been initialized. */
static seq_t next_cand[10] = {0};

static void ensure_seq_cap(size_t need)
{
    if (seq_cap < need) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 64;
        while (new_cap < need) new_cap <<= 1;
        seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

static void ensure_used_cap(size_t need)
{
    if (used_cap < need) {
        size_t new_cap = used_cap ? used_cap << 1 : 64;
        while (new_cap < need) new_cap <<= 1;
        used_arr = (seq_t *)realloc(used_arr, new_cap * sizeof(seq_t));
        used_cap = new_cap;
    }
}

/* Linear lookup to test whether v is already in the sequence. */
static bool is_used(seq_t v)
{
    for (size_t i = 0; i < used_len; ++i)
        if (used_arr[i] == v) return true;
    return false;
}

/* Return the smallest unused integer that contains digit d.
   Update next_cand[d] to point to this candidate. */
static seq_t candidate_for_digit(uint8_t d)
{
    seq_t c = next_cand[d];
    if (c == 0) c = d;        /* first possible candidate with digit d */
    while (is_used(c) || !has_digit(c, d)) ++c;
    next_cand[d] = c;
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined below offset */

    if (seq_len == 0) {        /* initialise first term a(1) = 1 */
        ensure_seq_cap(1);
        seq_arr[0] = 1;
        seq_len = 1;

        ensure_used_cap(1);
        used_arr[0] = 1;
        used_len = 1;
    }

    /* Compute terms up to requested index n */
    while (seq_len < (size_t)n) {
        seq_t prev = seq_arr[seq_len - 1];

        /* Get odd digits of the previous term */
        uint8_t odds[5];
        int odcnt;
        odd_digits(prev, odds, &odcnt);

        /* Find the smallest unused candidate among all such digits */
        seq_t best = 0;
        for (int i = 0; i < odcnt; ++i) {
            seq_t cand = candidate_for_digit(odds[i]);
            if (best == 0 || cand < best) best = cand;
        }

        /* Append best to the sequence and mark it used */
        ensure_seq_cap(seq_len + 1);
        seq_arr[seq_len++] = best;

        ensure_used_cap(used_len + 1);
        used_arr[used_len++] = best;

        /* Update next_cand for the odd digits present in the new term */
        uint8_t new_odds[5];
        int ndcnt;
        odd_digits(best, new_odds, &ndcnt);
        for (int i = 0; i < ndcnt; ++i) {
            uint8_t d = new_odds[i];
            if (next_cand[d] == 0) next_cand[d] = d;  /* initialise if needed */
            if (next_cand[d] == best) {
                seq_t c = best;
                do { ++c; } while (is_used(c) || !has_digit(c, d));
                next_cand[d] = c;
            }
        }
    }

    return seq_arr[n - 1];
}
