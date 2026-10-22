/*
 * A364935  a(1) = 1 and thereafter a(n) = sum of digits of the number from
 *          concatenation of a(n-1) on the left with leftmost digit of a(n-1)
 *          + k and concatenation on the right with rightmost digit of a(n-1)
 *          + k, where k is the number of times a(n-1) has appeared in the
 *          sequence.
 * https://oeis.org/A364935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, look, nonn
 * Formula:   a(1) = 1; for n>1, let k be the number of times a(n-1) has
 *            appeared so far (including that term). Let l be the leftmost
 *            digit of a(n-1) and r the rightmost. Define L = l + k and R = r
 *            + k. Then a(n) = digit_sum(a(n-1)) + digit_sum(L) + digit_sum(R)
 * Range:     u8 n <= 3363, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* compute the sum of the decimal digits of a value of type seq_t */
static inline seq_t digit_sum_local(seq_t v) {
    seq_t sum = 0;
    if (v == 0) return 0;
    while (v > 0) {
        sum += v % 10;
        v /= 10;
    }
    return sum;
}

/* recursive sequence a(n) as defined above. */
seq_t a(idx_t n) {
    if (n < 1) return 0;           /* undefined for n = 0, return 0 */

    /* static cache for computed terms (1‑based indexing) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored (cache_len>=1 after init) */
    static size_t cache_cap = 0;

    /* static tables to keep the current occurrence count of each
       distinct term that has appeared so far */
    static seq_t *uniq_vals = NULL;
    static unsigned *uniq_cnts = NULL;
    static size_t uniq_len = 0;
    static size_t uniq_cap = 0;

    /* initialise on first call */
    if (cache_len == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* store a(1) */
        cache[1] = (seq_t)1;
        cache_len = 1;

        uniq_cap = 4;
        uniq_vals = (seq_t *)malloc(uniq_cap * sizeof(seq_t));
        uniq_cnts = (unsigned *)malloc(uniq_cap * sizeof(unsigned));
        uniq_vals[0] = (seq_t)1;
        uniq_cnts[0] = 1;
        uniq_len = 1;
    }

    /* extend the cache up to the desired index */
    if (n > cache_len) {
        idx_t target = n;
        for (idx_t i = cache_len + 1; i <= target; ++i) {
            /* previous term */
            seq_t prev = cache[i - 1];

            /* number of times 'prev' has occurred so far (k) */
            unsigned k = 0;
            for (size_t u = 0; u < uniq_len; ++u)
                if (uniq_vals[u] == prev) { k = uniq_cnts[u]; break; }

            /* leftmost and rightmost digits of 'prev' */
            seq_t leftmost;
            if (prev == 0) {
                leftmost = 0;
            } else {
                seq_t tmp = prev;
                leftmost = tmp;
                while (leftmost >= 10) leftmost /= 10;
            }
            seq_t rightmost = prev % 10;

            /* digits after adding k */
            seq_t left_digit  = leftmost + (seq_t)k;
            seq_t right_digit = rightmost + (seq_t)k;

            /* new term (sum of digit sums of the three parts) */
            seq_t val = digit_sum_local(prev) +
                        digit_sum_local(left_digit) +
                        digit_sum_local(right_digit);

            /* grow cache array if necessary */
            if (cache_len + 1 >= cache_cap) {
                size_t newcap = cache_cap ? cache_cap * 2 : 4;
                while (newcap <= cache_len + 1) newcap *= 2;
                cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                cache_cap = newcap;
            }

            cache[i] = val;
            ++cache_len;

            /* update occurrence count for the new term */
            bool found = false;
            for (size_t u = 0; u < uniq_len; ++u) {
                if (uniq_vals[u] == val) {
                    ++uniq_cnts[u];
                    found = true;
                    break;
                }
            }
            if (!found) {
                if (uniq_len >= uniq_cap) {
                    size_t newcap = uniq_cap ? uniq_cap * 2 : 4;
                    uniq_vals = (seq_t *)realloc(uniq_vals, newcap * sizeof(seq_t));
                    uniq_cnts = (unsigned *)realloc(uniq_cnts, newcap * sizeof(unsigned));
                    uniq_cap = newcap;
                }
                uniq_vals[uniq_len] = val;
                uniq_cnts[uniq_len] = 1;
                ++uniq_len;
            }
        }
    }

    return cache[n];
}
