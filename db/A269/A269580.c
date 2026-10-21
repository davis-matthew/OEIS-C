/*
 * A269580  Number of length-n 0..5 arrays with no repeated value differing
 *          from the previous repeated value by more than one.
 * https://oeis.org/A269580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   diff_uint, idx_state
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of length‑n arrays over {0,…,5} with no repeated value
 * whose difference from the previous repeated value exceeds 1.
 * The sequence satisfies the 7‑term recurrence
 *   a(n)=20*a(n-1)-146*a(n-2)+440*a(n-3)-328*a(n-4)-470*a(n-5)-76*a(n-6)+5*a(n-7)
 * with initial values
 *   a(1)=6, a(2)=36, a(3)=216, a(4)=1276, a(5)=7456, a(6)=43184, a(7)=248324.
 * This implementation computes values via a small DP that encodes the
 * previous array element and the last repeated value.
 */

#include <stdlib.h>
#include <string.h>   /* for memset */
#include <stdbool.h>
#include "../utils/diff_uint.h"
#include "../utils/idx_state.h"

#define P         6U      /* number of possible values (0..5) */
#define R_NONE    6U      /* sentinel for “no repetition yet” */
#define STATES    (P*7U)  /* prev * 7 + last_rep (0..6) */

/* Function to compute a(n) */
seq_t a(idx_t n) {
    if (n < 1U) return 0U;   /* offset 1; behaviour below is unspecified */

    /* Static state for memoised computation */
    static idx_t current_len = 0U;           /* length of last computed DP (>=1) */
    static seq_t *cache = NULL;             /* array a[1] … a[n] stored as cache[0] … */
    static size_t cache_size = 0U;          /* allocated size of cache */
    static seq_t dp_curr[STATES];           /* current DP vector (length curr_len) */
    static seq_t dp_next[STATES];           /* temporary DP vector for next step */

    /* Initialisation for first call or when reset */
    if (current_len == 0U) {
        /* Allocate initial cache for length 1 */
        cache_size = 20U;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return 0U; /* allocation failure */

        /* Zero dp_curr */
        memset(dp_curr, 0, sizeof(dp_curr));

        /* Start with length = 1: prev can be any of 0..5, last_rep = R_NONE */
        for (unsigned prev = 0U; prev < P; ++prev) {
            dp_curr[idx_state(prev, R_NONE)] = 1U;
        }
        /* Store a(1) = 6 */
        cache[0] = 6U;
        current_len = 1U;
    }

    /* Extend cache and DP up to requested n */
    if ((idx_t)n > current_len) {
        /* Reallocate cache if necessary */
        if ((idx_t)n > cache_size) {
            size_t new_size = cache_size;
            while ((idx_t)n > new_size) new_size *= 2U;
            seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
            if (tmp == NULL) return 0U; /* allocation failure */
            cache = tmp;
            cache_size = new_size;
        }

        /* Iterate from current_len+1 up to n */
        for (idx_t len = current_len + 1U; len <= n; ++len) {
            /* Zero next DP vector */
            memset(dp_next, 0, sizeof(dp_next));

            /* For every state in dp_curr, propagate to next states */
            for (unsigned prev = 0U; prev < P; ++prev) {
                for (unsigned rep = 0U; rep <= P; ++rep) {
                    size_t state_idx = idx_state(prev, rep);
                    seq_t ways = dp_curr[state_idx];
                    if (ways == 0U) continue;

                    /* Try all possible next values v */
                    for (unsigned v = 0U; v < P; ++v) {
                        if (v == prev) { /* repetition */
                            if (rep == R_NONE || diff_uint(v, rep) <= 1U) {
                                size_t next_idx = idx_state(v, v);
                                dp_next[next_idx] += ways;
                            }
                        } else { /* non‑repetition */
                            size_t next_idx = idx_state(v, rep);
                            dp_next[next_idx] += ways;
                        }
                    }
                }
            }

            /* Compute total for this length and store */
            seq_t total = 0U;
            for (unsigned s = 0U; s < STATES; ++s) {
                total += dp_next[s];
            }
            cache[len - 1U] = total;

            /* Prepare for next iteration */
            memcpy(dp_curr, dp_next, sizeof(dp_curr));
        }
        current_len = n; /* now DP corresponds to length n */
    }

    /* Return requested value */
    return cache[n - 1U];
}
