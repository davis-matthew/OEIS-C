/*
 * A309882  Sum of the even parts appearing among the fourth largest parts of
 *          the partitions of n into 5 parts.
 * https://oeis.org/A309882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A309882: Sum of the even parts appearing among the fourth largest
 * parts of the partitions of n into 5 parts.
 *
 * The generating function is
 *   G(x) = 2*x^9*(1+x)*(1-x+x^2)*(1-x^3+x^6)*(1+x^3+x^6)
 *          /((1-x)*(1-x^8)*(1-x^10))^2
 * 
 * From this we obtain a linear recurrence of order 38:
 *   a(n) = –∑_{k=1}^{38} Q[k] * a(n-k),   n ≥ 38
 * where Q(x) = (1-2x+x^2)(1-2x^8+x^16)(1-2x^10+x^20)
 * and Q[0] = 1, Q[k] (k ≥ 1) are the coefficients of Q(x).
 * 
 * For n < 38 the sequence is computed by a direct enumeration of the
 * partitions into five positive parts.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Recurrence coefficients ------------------------------------------------ */
static int64_t Q[39] = {0};

static void init_Q(void)
{
    static int done = 0;
    if (done) return;
    /* multiply (1-2x+x^2) * (1-2x^8+x^16) * (1-2x^10+x^20) */

    /* first poly: (1-2x+x^2) */
    int64_t poly1[3] = {1, -2, 1};

    /* second poly: (1-2x^8+x^16) */
    int64_t poly2[17] = {0};
    poly2[0] = 1;
    poly2[8] = -2;
    poly2[16] = 1;

    /* third poly: (1-2x^10+x^20) */
    int64_t poly3[21] = {0};
    poly3[0] = 1;
    poly3[10] = -2;
    poly3[20] = 1;

    /* convolution helper */
    int64_t tmp1[40] = {0};
    int64_t tmp2[40] = {0};
    int i, j, k, len, newlen;

    /* tmp1 = poly1 */
    for (i = 0; i < 3; ++i) tmp1[i] = poly1[i];
    len = 3;

    /* tmp1 *= poly2 */
    newlen = len + 17 - 1;
    for (i = 0; i < newlen; ++i) tmp2[i] = 0;
    for (i = 0; i < len; ++i)
        for (j = 0; j < 17; ++j)
            tmp2[i + j] += tmp1[i] * poly2[j];
    for (i = 0; i < newlen; ++i) tmp1[i] = tmp2[i];
    len = newlen;

    /* tmp1 *= poly3 */
    newlen = len + 21 - 1;
    for (i = 0; i < newlen; ++i) tmp2[i] = 0;
    for (i = 0; i < len; ++i)
        for (j = 0; j < 21; ++j)
            tmp2[i + j] += tmp1[i] * poly3[j];
    for (i = 0; i <= newlen; ++i) Q[i] = tmp2[i];

    done = 1;
}

/* ------------------------------------------------------------------ */
/* Enumeration of initial terms up to n = 37 ------------------------------------------------ */
static void compute_initial_terms(seq_t *init)
{
    for (idx_t n = 0; n < 38; ++n) {
        seq_t sum = 0;
        if (n < 5) {
            init[n] = 0;
            continue;
        }
        /* enumerate a >= b >= c >= d >= e >= 1 */
        for (idx_t a = 1; a <= n - 4; ++a) {
            for (idx_t b = 1; b <= a; ++b) {
                for (idx_t c = 1; c <= b; ++c) {
                    for (idx_t d = 1; d <= c; ++d) {
                        idx_t e = n - (a + b + c + d);
                        if (e < 1 || e > d) continue;
                        if ((d & 1) == 0) sum += d; /* d even */
                    }
                }
            }
        }
        init[n] = sum;
    }
}

/* ------------------------------------------------------------------ */
/* Main function ------------------------------------------------ */
seq_t a(idx_t n)
{
    /* cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_size = 0;  /* number of terms stored (>=3 means initialized) */

    /* whatever n is, we need at least min 38 for recurrence */
    const size_t MIN_INIT = 38;

    /* initialise recurrence coefficients once */
    init_Q();

    /* ensure cache large enough for at least MIN_INIT terms */
    if (cache_size < MIN_INIT) {
        size_t new_cap = MIN_INIT;
        if (new_cap < (size_t)(n + 1)) new_cap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
        /* compute initial 38 terms only once */
        compute_initial_terms(cache);
        cache_size = 38;
    }

    /* grow cache if needed */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)(n + 1)) new_cap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute terms up to n using recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 38) {
            /* already computed in initial block */
            continue;
        }
        /* recurrence: a(i) = - Σ_{k=1}^{38} Q[k] * a(i-k) */
        __int128 sum = 0;
        for (int k = 1; k <= 38; ++k) {
            sum -= (__int128)Q[k] * (__int128)cache[i - k];
        }
        cache[i] = (seq_t)sum;
    }
    cache_size = n + 1;

    return cache[n];
}
