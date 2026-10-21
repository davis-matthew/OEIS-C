/*
 * A272144  Convolution of A000217 and A001045.
 * https://oeis.org/A272144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) - 2*a(n-3) + 5*a(n-4) - 2*a(n-5) a(0)
 *            = 0, a(1) = 0, a(2) = 1, a(3) = 4, a(4) = 12
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Maximum n we can pre‑allocate statically.
   For larger n a dynamic allocation is used. */
#define DEFAULT_MAX_N 10000

/* Static cache for n <= DEFAULT_MAX_N */
static seq_t static_cache[DEFAULT_MAX_N + 1] = {0};
static idx_t static_cache_len = 0;

/* Dynamic cache for n > DEFAULT_MAX_N */
static seq_t *dynamic_cache = NULL;
static idx_t dynamic_cache_len = 0;
static idx_t dynamic_cache_cap = 0;

/* Helper: compute values up to n in the static cache */
static void compute_static_up_to(idx_t n)
{
    if (static_cache_len > n)
        return;

    for (idx_t i = static_cache_len; i <= n; ++i) {
        if (i <= 4) {
            /* initial values */
            switch (i) {
                case 0: static_cache[i] = (seq_t)0; break;
                case 1: static_cache[i] = (seq_t)0; break;
                case 2: static_cache[i] = (seq_t)1; break;
                case 3: static_cache[i] = (seq_t)4; break;
                case 4: static_cache[i] = (seq_t)12; break;
            }
        } else {
            seq_t val = 4 * static_cache[i - 1];
            val -= 4 * static_cache[i - 2];
            val -= 2 * static_cache[i - 3];
            val += 5 * static_cache[i - 4];
            val -= 2 * static_cache[i - 5];
            static_cache[i] = val;
        }
    }
    static_cache_len = n + 1;
}

/* Helper: compute values up to n in the dynamic cache */
static void compute_dynamic_up_to(idx_t n)
{
    if (dynamic_cache_len > n)
        return;

    if (dynamic_cache_cap <= n) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(dynamic_cache, new_cap * sizeof(seq_t));
        dynamic_cache = tmp;
        dynamic_cache_cap = new_cap;
    }

    for (idx_t i = dynamic_cache_len; i <= n; ++i) {
        if (i <= 4) {
            /* initial values */
            switch (i) {
                case 0: dynamic_cache[i] = (seq_t)0; break;
                case 1: dynamic_cache[i] = (seq_t)0; break;
                case 2: dynamic_cache[i] = (seq_t)1; break;
                case 3: dynamic_cache[i] = (seq_t)4; break;
                case 4: dynamic_cache[i] = (seq_t)12; break;
            }
        } else {
            seq_t val = 4 * dynamic_cache[i - 1];
            val -= 4 * dynamic_cache[i - 2];
            val -= 2 * dynamic_cache[i - 3];
            val += 5 * dynamic_cache[i - 4];
            val -= 2 * dynamic_cache[i - 5];
            dynamic_cache[i] = val;
        }
    }
    dynamic_cache_len = n + 1;
}

/* Public entry point */
seq_t a(idx_t n)
{
    if (n <= (idx_t)DEFAULT_MAX_N) {
        compute_static_up_to(n);
        return static_cache[n];
    } else {
        compute_dynamic_up_to(n);
        return dynamic_cache[n];
    }
}
