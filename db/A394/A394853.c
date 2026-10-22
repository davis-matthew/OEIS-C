/*
 * A394853  Expansion of g^7/(6-5*g)^4, where g = 1+x*g^6 is the g.f. of
 *          A002295.
 * https://oeis.org/A394853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 5^k * binomial(k+2,2) * binomial(6*n+6, n-k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache of powers of 5 */
static seq_t *pow5 = NULL;
static size_t pow5_cap = 0;
static idx_t pow5_size = 0;

static void ensure_pow5_up_to(idx_t n) {
    if (pow5 == NULL) {
        pow5_cap = 128;
        pow5 = (seq_t*)malloc(pow5_cap * sizeof(seq_t));
        pow5_size = 0;
    }
    if (pow5_size <= n) {
        if (pow5_cap <= (size_t)n) {
            size_t new_cap = pow5_cap;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            pow5 = (seq_t*)realloc(pow5, new_cap * sizeof(seq_t));
            pow5_cap = new_cap;
        }
        for (idx_t i = pow5_size; i <= n; ++i) {
            if (i == 0)
                pow5[0] = (seq_t)1;
            else
                pow5[i] = pow5[i - 1] * (seq_t)5;
        }
        pow5_size = n + 1;
    }
}

/* cache of a(n) values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;

static void ensure_cache_up_to(idx_t n) {
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_size = 0;
    }

    while (cache_size <= n) {
        idx_t cur = cache_size;
        ensure_pow5_up_to(cur);

        seq_t sum = 0;
        seq_t choose = 1;                     /* C(6*cur+6, 0) */
        idx_t N = 6 * cur + 6;                /* total in binomial */

        for (idx_t m = 0; m <= cur; ++m) {    /* m = n - k  */
            idx_t k = cur - m;                /* k = 0..cur   */

            /* binomial(k+2,2) = (k+2)(k+1)/2 */
            seq_t bin_k = ((seq_t)(k + 2)) * ((seq_t)(k + 1)) / (seq_t)2;

            seq_t term = pow5[k];
            term = term * bin_k;
            term = term * choose;

            sum = sum + term;

            if (m < cur) {
                /* choose for next m+1: C(N, m+1) = C(N, m) * (N - m) / (m+1) */
                choose = choose * (seq_t)(N - m);
                choose = choose / (seq_t)(m + 1);
            }
        }

        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap << 1;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        cache[cache_size] = sum;
        ++cache_size;
    }
}

seq_t a(idx_t n) {
    ensure_cache_up_to(n);
    return cache[n];
}
