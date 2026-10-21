/*
 * A274583  Expansion of (1 + x + x^2 - x^3 - x^4 + x^6)/((1 - x)^3*(1 + x +
 *          x^2)^2).
 * https://oeis.org/A274583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-3) - 2*a(n-4) - a(n-6) + a(n-7), with
 *            initial values a(0)=1, a(1)=2, a(2)=3, a(3)=4, a(4)=5, a(5)=7,
 *            a(6)=9, a(7)=10 All arithmetic is done in the seq_t type; no
 *            floating‑point or external libraries are required. The function
 *            memoises previously computed terms in a static ar...
 * Range:     u8 n <= 44, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache   = NULL;   /* array of already computed values   */
static size_t seq_cap    = 0;       /* current capacity of seq_cache      */
static size_t seq_size   = 0;       /* next index that is free (i.e. size)*/

seq_t a(idx_t n) {
    /* Return a(n) for all n >= 0 */
    static const seq_t base[8] = {1,2,3,4,5,7,9,10};

    if (n < 8) return base[(size_t)n];

    /* Initialise cache on first call */
    if (seq_cache == NULL) {
        seq_cap = 8;
        seq_cache = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        if (!seq_cache) return 0; /* Allocation failed – program should abort */
        for (size_t i = 0; i < 8; ++i) seq_cache[i] = base[i];
        seq_size = 8;
    }

    /* Grow cache if needed */
    if ((size_t)n + 1 > seq_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            free(seq_cache);
            seq_cache = NULL;
            seq_cap   = 0;
            seq_size  = 0;
            return 0; /* Allocation failed – program should abort */
        }
        seq_cache = tmp;
        seq_cap   = new_cap;
    }

    /* Compute missing terms */
    for (size_t i = seq_size; i <= (size_t)n; ++i) {
        seq_t a_nm1 = seq_cache[i-1];
        seq_t a_nm3 = seq_cache[i-3];
        seq_t a_nm4 = seq_cache[i-4];
        seq_t a_nm6 = seq_cache[i-6];
        seq_t a_nm7 = seq_cache[i-7];

        seq_t two_a_nm3 = a_nm3 + a_nm3;
        seq_t two_a_nm4 = a_nm4 + a_nm4;

        seq_t sum_pos = a_nm1 + two_a_nm3 + a_nm7;
        seq_t sum_neg = two_a_nm4 + a_nm6;

        seq_cache[i] = sum_pos - sum_neg;  /* unsigned subtraction is safe */
    }

    seq_size = (size_t)n + 1;
    return seq_cache[(size_t)n];
}
