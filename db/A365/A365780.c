/*
 * A365780  Lexicographically earliest finite sequence of nonprime numbers
 *          such that a(n) + [the next digit] is a prime number.
 * https://oeis.org/A365780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Generate the lexicographically earliest sequence of
 * distinct nonprime integers {a(n)} such that for every n >= 1
 *      a(n) + first_digit(a(n+1)) is prime.
 *
 * The sequence is finite; it stops when no further term can be
 * appended.  This code lazily builds the entire finite sequence
 * on the first call and then answers any subsequent query
 * (in any order) in O(1) time.  All computations are done with
 * the native seq_t type; no external helper functions are used. */

#include <stdbool.h>
#include <stdlib.h>

/* helper: check whether a native integer is prime (for small values) */
static bool is_prime_small(seq_t n)
{
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (seq_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

/* helper: obtain the first (most significant) decimal digit of x */
static seq_t first_digit(seq_t x)
{
    while (x >= 10) x /= 10;
    return x;
}

/* helper: check if x is already in the sequence (linear search) */
static bool used_in_seq(const seq_t *seq, idx_t len, seq_t x)
{
    for (idx_t i = 0; i < len; ++i)
        if (seq[i] == x) return true;
    return false;
}

/* main sequence generator */
seq_t a(idx_t n)
{
    /* static storage for the generated sequence */
    static seq_t *seq = NULL;
    static idx_t   len = 0;   /* number of elements stored */
    static size_t  cap = 0;   /* allocated capacity */

    /* build the full sequence once */
    if (seq == NULL) {
        /* initial allocation */
        cap = 128;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        len = 0;

        /* add the first element (offset 1) */
        seq[len++] = 1;

        /* keep generating until we cannot find a next term */
        while (true) {
            seq_t prev = seq[len - 1];
            seq_t cand;
            const seq_t MAX_CAND = 5000;   /* limit to keep search finite */
            bool found = false;

            for (cand = 1; cand <= MAX_CAND; ++cand) {
                if (is_prime_small(cand)) continue;        /* must be nonprime */
                if (used_in_seq(seq, len, cand)) continue; /* distinctness */

                seq_t d = first_digit(cand);
                seq_t sum = prev + d;
                if (!is_prime_small(sum)) continue;       /* sum must be prime */

                /* valid candidate found */
                if (len == cap) {                       /* extend storage */
                    cap *= 2;
                    seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
                }
                seq[len++] = cand;
                found = true;
                break;                                   /* proceed to next position */
            }

            if (!found) break;   /* no further term -> sequence terminates */
        }
    }

    /* answer the query */
    if (n >= 1 && n <= len) return seq[n - 1];
    return 0; /* out of bounds (should not occur for valid queries) */
}
