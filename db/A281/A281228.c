/*
 * A281228  Expansion of (Sum_{k>=0} x^(3^k))^2 [even terms only].
 * https://oeis.org/A281228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (i,j) with 3^i + 3^j = 2n
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A281228: number of ways to write 2n as an ordered sum of two powers of 3 */

#include <stdlib.h>
#include <stdint.h>

/* cache for computed values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* dynamic array of powers of 3 */
static seq_t *powers = NULL;
static size_t powers_len = 0;
static size_t powers_cap = 0;

/* Helper to grow a dynamic array of seq_t */
static void grow_seq_arr(seq_t **arr, size_t *len, size_t *cap, size_t new_cap) {
    if (*cap < new_cap) {
        size_t alloc = new_cap > 0 ? new_cap : 4;
        seq_t *tmp = (seq_t *)realloc(*arr, alloc * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = alloc;
    }
}

/* Ensure powers_of_3 contains all values <= max_val */
static void ensure_powers(seq_t max_val) {
    if (powers_len == 0) {
        /* start with 1 */
        grow_seq_arr(&powers, &powers_len, &powers_cap, 1);
        powers[0] = 1;
        powers_len = 1;
    }
    while (1) {
        seq_t last = powers[powers_len - 1];
        /* check for overflow: if last*3 would overflow seq_t, break */
        seq_t next = last * 3;
        if (next / 3 != last) break; /* overflow */
        if (next > max_val) break;
        grow_seq_arr(&powers, &powers_len, &powers_cap, powers_len + 1);
        powers[powers_len++] = next;
    }
}

seq_t a(idx_t n) {
    /* ensure cache is large enough */
    if ((size_t)n >= cache_len) {
        size_t new_cap = cache_len == 0 ? 64 : cache_len * 2;
        while (new_cap <= (size_t)n) new_cap *= 2;
        grow_seq_arr(&cache, &cache_len, &cache_cap, new_cap);
    }
    /* return cached value if already computed */
    if (cache[n] != 0) return cache[n];  /* cache[0]==0 is valid but will be recomputed */

    seq_t t = (seq_t)n * 2;  /* target sum */
    seq_t count = 0;

    /* If t is odd, no solutions */
    if ((t & 1) == 0) {
        /* prepare powers of 3 up to t */
        ensure_powers(t);

        for (size_t i = 0; i < powers_len; ++i) {
            seq_t pi = powers[i];
            if (pi > t) break;
            for (size_t j = 0; j < powers_len; ++j) {
                seq_t sum = pi + powers[j];
                if (sum == t) {
                    count += 1;
                }
                if (sum > t) break; /* powers are increasing */
            }
        }
    }

    cache[n] = count;
    return count;
}
