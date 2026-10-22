/*
 * A383789  a(1) = 1; for n > 1, a(n) is the smallest positive integer not
 *          already in the sequence such that it shares at least one digit
 *          with a(n-1), and it has a different number of digits from a(n-1).
 * https://oeis.org/A383789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 159, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_counts, digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A383789: 
 * a(1) = 1
 * For n > 1, a(n) is the smallest positive integer not already in the sequence
 * that shares at least one decimal digit with a(n-1) and has a different
 * number of decimal digits from a(n-1).
 *
 * The implementation builds the sequence lazily and memoises all terms
 * computed so far.  Each new term is found by testing candidates
 * sequentially starting from 1 until the conditions are satisfied.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
/* Headers for helper functions that operate on seq_t */
#include "../utils/digit_len.h"
#include "../utils/digit_counts.h"

static seq_t *seq = NULL;           /* array holding the sequence values      */
static size_t seq_sz  = 0;          /* number of terms stored                */
static size_t seq_cap = 0;          /* capacity of the array                */

/* Ensure the array has room for at least one more element. */
static void grow_seq(void)
{
    if (seq_sz == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 64;
        seq_t *newp = (seq_t *)realloc(seq, sizeof(seq_t) * new_cap);
        if (!newp) abort();                    /* allocation failure */
        seq = newp;
        seq_cap = new_cap;
    }
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 */

    /* initialise first term if needed */
    if (seq_sz == 0) {
        grow_seq();
        seq[0] = (seq_t)1;
        seq_sz = 1;
    }

    /* return cached value if already computed */
    if ((idx_t)seq_sz >= n) {
        return seq[n - 1];
    }

    /* compute terms up to the requested index */
    while ((idx_t)seq_sz < n) {
        seq_t prev = seq[seq_sz - 1];
        unsigned prev_len = digit_len(prev);

        /* digit counts of the previous term */
        unsigned char prev_cnt[10] = {0};
        digit_counts(prev, prev_cnt);

        seq_t cand = 1;                   /* lowest possible candidate */

        /* Search for the next term satisfying the rules */
        while (true) {
            /* Check if candidate is already used */
            bool used = false;
            for (size_t i = 0; i < seq_sz; ++i) {
                if (seq[i] == cand) {
                    used = true;
                    break;
                }
            }
            if (!used) {
                unsigned cand_len = digit_len(cand);
                if (cand_len != prev_len) {
                    unsigned char cand_cnt[10];
                    digit_counts(cand, cand_cnt);
                    bool share = false;
                    for (int d = 0; d < 10 && !share; ++d) {
                        if (prev_cnt[d] && cand_cnt[d]) share = true;
                    }
                    if (share) break;          /* candidate found */
                }
            }
            ++cand;                          /* try next integer */
        }

        /* Append the found candidate to the sequence */
        grow_seq();
        seq[seq_sz++] = cand;
    }

    return seq[n - 1];
}
