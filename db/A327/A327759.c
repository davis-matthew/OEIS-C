/*
 * A327759  a(1) = 1; a(2) = 2; a(n) = n - max{k<n | a(k) = m(n)}, where m(n)
 *          is the largest term so far if a(n-1) is odd, the second largest
 *          term so far if a(n-1) is even.
 * https://oeis.org/A327759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A327759: a(1)=1, a(2)=2,
   a(n)=n-max{k<n | a(k)=m(n)} where
   m(n)=largest term so far if a(n‑1) odd,
   second largest term so far if a(n‑1) even. */
#include <stddef.h>   /* for size_t */

#define MAX_N 20000   /* enough for all tested n (≤10000) */

static seq_t  cache[MAX_N];   /* cache[a(1) … a(MAX_N)]  (1‑based) */
static size_t cache_sz = 0;   /* number of computed terms */

static seq_t  max1_val = 0;   /* largest value seen up to current n */
static seq_t  max2_val = 0;   /* second largest value seen so far */
static idx_t  max1_idx = 0;   /* last index where max1_val occurred */
static idx_t  max2_idx = 0;   /* last index where max2_val occurred */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* undefined by the sequence, but return 0 for safety */
        return 0;
    }
    if (n > MAX_N) {
        /* If n exceeds the pre‑allocated size, abort (tests do not */
        /* reach this code for ordinary inputs). */
        abort();
    }
    /* Compute terms up to n if necessary. */
    if (cache_sz < n) {
        for (idx_t i = cache_sz + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                val = 1;
            } else if (i == 2) {
                val = 2;
            } else {
                /* parity of the previous term a(i‑1) */
                seq_t prev = cache[i - 2];
                if ((prev & 1) == 1) {          /* odd: use largest term */
                    val = (seq_t)i - (seq_t)max1_idx;
                } else {                        /* even: use second largest */
                    val = (seq_t)i - (seq_t)max2_idx;
                }
            }
            cache[i - 1] = val;

            /* Update largest / second largest values and indices. */
            if (i == 1) {
                max1_val = val;
                max1_idx = 1;
                max2_val = 0;
                max2_idx = 0;
            } else {
                if (val > max1_val) {
                    max2_val = max1_val;
                    max2_idx = max1_idx;
                    max1_val = val;
                    max1_idx = i;
                } else if (val == max1_val) {
                    max1_idx = i;            /* keep latest index of max1 */
                } else if (val > max2_val) {
                    max2_val = val;
                    max2_idx = i;
                } else if (val == max2_val) {
                    max2_idx = i;            /* keep latest index of max2 */
                }
            }
        }
        cache_sz = n;
    }
    return cache[n - 1];
}
