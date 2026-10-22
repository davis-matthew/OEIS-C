/*
 * A304237  a(n) is the smallest positive integer not yet in the sequence that
 *          is obtained when the first and last digits of a(n-1) are exchanged
 *          and used in a(n) in the exchanged order (but not necessarily
 *          adjacent); a(1)=10.
 * https://oeis.org/A304237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   has_pattern, is_in_seen
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non‑formula generation for OEIS A304237.
   a(1)=10 and thereafter a(n) is the smallest positive integer not yet seen
   whose decimal representation contains the last digit of a(n‑1) before the
   first digit of a(n‑1).  The function memoises results so calls in any
   order are fast.  It works for arbitrary‑precision seq_t classes as well
   as native integer types. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_pattern.h"
#include "../utils/is_in_seen.h"

/* Return the most significant decimal digit of x. */
static seq_t first_digit(seq_t x)
{
    while (x >= (seq_t)10) x /= (seq_t)10;
    return x;
}

/* Return the least significant decimal digit of x. */
static seq_t last_digit(seq_t x)
{
    return x % (seq_t)10;
}

/* Reallocate an array of seq_t to at least 'need' capacity.
   Copies existing elements and frees the old array. */
static void ensure_capacity(seq_t **ptr, size_t *cap, size_t *len, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap) ? (*cap << 1) : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    for (size_t i = 0; i < *len; ++i) tmp[i] = (*ptr)[i];
    free(*ptr);
    *ptr = tmp;
    *cap = new_cap;
}

/* Memoised sequence storage. */
static seq_t *seq       = NULL;
static size_t seq_cap   = 0;
static size_t seq_len   = 0;

static seq_t *seen      = NULL;
static size_t seen_cap  = 0;
static size_t seen_len  = 0;

/* Main function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for offset 1 */

    /* Generate terms up to n if necessary. */
    if (seq_len < (size_t)n) {
        /* Grow sequence storage. */
        ensure_capacity(&seq, &seq_cap, &seq_len, (size_t)n);

        /* Grow seen storage – at least twice the sequence length to keep load factor reasonable. */
        ensure_capacity(&seen, &seen_cap, &seen_len, (size_t)(2 * n));

        /* Initialise first term if sequence is empty. */
        if (seq_len == 0) {
            seq[0]   = (seq_t)10;
            seen[0]  = (seq_t)10;
            seq_len  = 1;
            seen_len = 1;
        }

        /* Generate subsequent terms. */
        while (seq_len < (size_t)n) {
            seq_t prev = seq[seq_len - 1];
            seq_t f    = first_digit(prev);
            seq_t l    = last_digit(prev);

            /* Search for smallest candidate not yet seen that satisfies the pattern. */
            seq_t cand = (seq_t)1;
            for (;;) {
                if (!is_in_seen(cand, seen, seen_len) && has_pattern(cand, l, f)) {
                    seq[seq_len] = cand;
                    seen[seen_len++] = cand;
                    ++seq_len;
                    break;
                }
                ++cand;
            }
        }
    }
    return seq[n - 1];
}
