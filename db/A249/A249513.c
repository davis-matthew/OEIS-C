/*
 * A249513  Expansion of -(4*x*sqrt(4*x^2+1)+8*x^2+1)/((2*x^2-1)*sqrt(4*x^2+1)
 *          +4*x^3+x).
 * https://oeis.org/A249513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 18, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence used:
 *     -n*(3*n-4)*a(n)
 *    -(n-1)*(3*n-7)*a(n-1)
 *    +(3*n^2+8*n+24)*a(n-2)
 *    +(3*n^2+2*n+19)*a(n-3)
 *    +20*(3*n+2)*(n-3)*a(n-4)
 *    +20*(3*n-1)*(n-4)*a(n-5) = 0
 *
 *  Initial values:
 *     a(0) = 1, a(1) = 5, a(2) = 13, a(3) = 25, a(4) = 51
 *
 *  This implementation memoizes results up to the largest required n
 *  and computes each new term using only unsigned arithmetic, avoiding
 *  negative intermediate values by separating the positive and negative
 *  contributions of the recurrence.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;   /* number of terms stored (next index to compute) */

    /* enlarge cache if needed */
    if (n >= cache_cap) {
        size_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 8;
        while (new_cap <= (size_t)n + 1) new_cap *= 2;  /* +1 to include n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – free any allocated memory */
            free(cache);
            cache = NULL;
            cache_cap = 0;
            cache_len = 0;
            return 0;        /* should never happen for valid builds */
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0)          val = (seq_t)1;
        else if (i == 1)     val = (seq_t)5;
        else if (i == 2)     val = (seq_t)13;
        else if (i == 3)     val = (seq_t)25;
        else if (i == 4)     val = (seq_t)51;
        else {
            /* compute recurrence */
            seq_t an1 = cache[i - 1];
            seq_t an2 = cache[i - 2];
            seq_t an3 = cache[i - 3];
            seq_t an4 = cache[i - 4];
            seq_t an5 = cache[i - 5];

            /* coefficients – computed in idx_t to avoid overflow, then cast */
            idx_t n_i = i;
            seq_t c_n2 = (seq_t)(3 * n_i * n_i + 8 * n_i + 24);     /* for a(n-2) */
            seq_t c_n3 = (seq_t)(3 * n_i * n_i + 2 * n_i + 19);     /* for a(n-3) */
            seq_t c_n4 = (seq_t)(20 * (3 * n_i + 2) * (n_i - 3));    /* for a(n-4) */
            seq_t c_n5 = (seq_t)(20 * (3 * n_i - 1) * (n_i - 4));    /* for a(n-5) */
            seq_t c_n1 = (seq_t)((n_i - 1) * (3 * n_i - 7));        /* for a(n-1) */

            /* positive part of numerator */
            seq_t pos = an2 * c_n2;
            pos += an3 * c_n3;
            pos += an4 * c_n4;
            pos += an5 * c_n5;

            /* negative part of numerator (subtracted) */
            seq_t neg = an1 * c_n1;

            seq_t num = pos - neg;
            seq_t denom = (seq_t)n_i * (seq_t)(3 * n_i - 4);

            val = num / denom;  /* exact division by construction */
        }
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
