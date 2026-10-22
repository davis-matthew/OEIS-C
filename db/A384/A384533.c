/*
 * A384533  Inventory sequence starting a(1) = 1 and thereafter recording the
 *          number of terms which are the product of 0,1,2,... primes (taken
 *          with multiplicity). The count resets following a term = 0.
 * https://oeis.org/A384533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 771, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A384533:
   a(1) = 1. For n > 1, let m be the current prime‑factor count (starting at 0,
   increasing by one each step, resetting to 0 after a term equals 0).
   Then a(n) = number of preceding terms that are products of exactly m primes
   (counting multiplicity). The value 0 itself is not counted as a product
   of primes.  This algorithm maintains the counts incrementally.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n) {
    /* Static state for memoisation */
    static bool initialized = false;
    static seq_t *seq = NULL;      /* array of computed terms          */
    static size_t seq_cap = 0;     /* capacity of seq array            */
    static size_t seq_len = 0;     /* number of terms stored           */

    static seq_t *cnt = NULL;      /* counter for each prime‑factor count */
    static size_t cnt_cap = 0;     /* capacity of cnt array             */
    static size_t cnt_len = 0;     /* logical length of counts array    */

    static idx_t m = 0;            /* inventory counter (current omega) */

    if (!initialized) {
        initialized = true;

        /* initialise sequence with a(1) = 1 */
        seq_cap = 1;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;
        seq_len = 1;

        /* initialise counter array for omega = 0 */
        cnt_cap = 1;
        cnt = (seq_t *)malloc(cnt_cap * sizeof(seq_t));
        cnt[0] = (seq_t)1;      /* one term (the initial 1) has 0 prime factors */
        cnt_len = 1;

        m = 0; /* first inventory counter */
    }

    /* Grow the sequence up to the required index */
    while (seq_len < (size_t)n) {
        seq_t next_val = 0;

        /* a(seq_len+1) = number of terms with exactly m prime factors */
        if ((size_t)m < cnt_len)
            next_val = cnt[(size_t)m];

        /* Append the new term to the sequence */
        if (seq_len == seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 1;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            seq_cap = new_cap;
        }
        seq[seq_len] = next_val;
        seq_len++;

        /* Update counter for this term if it is non‑zero */
        if (next_val != (seq_t)0) {
            idx_t omega = (idx_t)count_prime_factors(next_val);

            /* Ensure the counter array can hold omega */
            if ((size_t)omega >= cnt_len) {
                size_t new_len = (size_t)omega + 1;
                /* Grow capacity if necessary */
                while (cnt_cap <= new_len)
                    cnt_cap = cnt_cap ? cnt_cap * 2 : 1;
                cnt = (seq_t *)realloc(cnt, cnt_cap * sizeof(seq_t));
                /* Initialise new entries to 0 */
                for (size_t i = cnt_len; i < new_len; ++i)
                    cnt[i] = (seq_t)0;
                cnt_len = new_len;
            }
            cnt[(size_t)omega]++;   /* increment count for this omega */
        }

        /* Advance inventory counter, resetting after a zero */
        if (next_val == (seq_t)0)
            m = 0;
        else
            m++;
    }

    /* Return the requested term (1‑based index) */
    return seq[(size_t)n - 1];
}
