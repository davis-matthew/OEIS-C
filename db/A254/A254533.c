/*
 * A254533  Number of length n 1..(2+2) arrays with no leading partial sum
 *          equal to a prime.
 * https://oeis.org/A254533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 42, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A254533: Number of length n arrays over {1,2,3,4} with no
   prefix sum equal to a prime.  This function implements a dynamic‑
   programming recurrence that for each length keeps the number of
   admissible prefixes for every possible current sum.  */

#include <stdbool.h>
#include <stdlib.h>

/* helpers for allocating arrays that work in both C and C++ */
#if defined(__cplusplus)
#define ALLOC_SEQ(type, count) (new type[count])
#define FREE_SEQ(p)          (delete [] (p))
#else
#define ALLOC_SEQ(type, count) ((type*)calloc((count), sizeof(type)))
#define FREE_SEQ(p)          (free(p))
#endif

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0;    /* undefined for n<1, return 0 */
    }

    /* maximum possible sum after n steps is 4*n */
    idx_t max_sum = 4 * n;
    idx_t primes_size = max_sum + 1;

    /* allocate and compute prime table up to max_sum */
    bool *is_prime = (bool*)malloc(primes_size * sizeof(bool));
    if (!is_prime) exit(1);

    for (idx_t i = 0; i < primes_size; ++i) is_prime[i] = true;
    if (primes_size > 0) is_prime[0] = false;
    if (primes_size > 1) is_prime[1] = false;
    for (idx_t p = 2; p * p <= max_sum; ++p) {
        if (is_prime[p]) {
            for (idx_t q = p * p; q <= max_sum; q += p) {
                is_prime[q] = false;
            }
        }
    }

    /* DP arrays: dp_prev holds counts for current prefix sum at length l-1,
       dp_cur for length l.  */
    seq_t *dp_prev = ALLOC_SEQ(seq_t, primes_size);
    seq_t *dp_cur  = ALLOC_SEQ(seq_t, primes_size);
    if (!dp_prev || !dp_cur) exit(1);

    /* zero initialize (necessary for C++ with non‑trivial seq_t) */
    for (idx_t i = 0; i < primes_size; ++i) {
        dp_prev[i] = 0;
        dp_cur[i]  = 0;
    }
    dp_prev[0] = 1;   /* empty prefix has sum 0 and is admissible */

    seq_t total = 0;

    for (idx_t len = 1; len <= n; ++len) {
        idx_t cur_max = 4 * len;
        /* zero dp_cur up to cur_max */
        for (idx_t s = 0; s <= cur_max; ++s) dp_cur[s] = 0;

        /* transition from previous length */
        idx_t prev_max = 4 * (len - 1);
        for (idx_t s = 0; s <= prev_max; ++s) {
            seq_t val = dp_prev[s];
            if (val == 0) continue;
            for (int d = 1; d <= 4; ++d) {
                idx_t ns = s + d;
                if (ns > cur_max) break;
                if (is_prime[ns]) continue;   /* skip prime partial sums */
                dp_cur[ns] += val;
            }
        }

        /* sum all admissible final sums for this length */
        total = 0;
        for (idx_t s = 0; s <= cur_max; ++s) {
            if (!is_prime[s]) total += dp_cur[s];
        }

        /* swap dp arrays for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev   = dp_cur;
        dp_cur    = tmp;
    }

    /* clean up */
    FREE_SEQ(dp_prev);
    FREE_SEQ(dp_cur);
    free(is_prime);

    return total;
}
