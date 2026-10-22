/*
 * A355100  E.g.f. A(x) satisfies A(x) = 1 + 2 * x * A(exp(x) - 1).
 * https://oeis.org/A355100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A355100
 *  Recurrence:  a(0) = 1
 *                a(n) = 2 * n * Σ_{k=0}^{n-1} Stirling2(n-1,k) * a(k)
 */
#include <stdlib.h>

static seq_t *a_cache = NULL;   /* cache of a(n) values */
static size_t a_cap   = 0;       /* allocated capacity of a_cache */
static idx_t  a_max   = 0;       /* highest n computed so far */

static seq_t *s_cache = NULL;   /* cache of Stirling numbers of the second kind */
static size_t s_cap   = 0;       /* allocated capacity of s_cache */

seq_t a(idx_t n)
{
    /* base case */
    if (n == 0)
        return (seq_t)1;

    /* first call – initialise caches */
    if (a_cache == NULL) {
        a_cap = 1;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        if (a_cache == NULL) abort();
        a_cache[0] = (seq_t)1;
        a_max = 0;
    }
    if (s_cache == NULL) {
        s_cap = 1;
        s_cache = (seq_t *)malloc(s_cap * sizeof(seq_t));
        if (s_cache == NULL) abort();
        s_cache[0] = (seq_t)1;   /* S(0,0) = 1 */
    }

    /* enlarge caches if we need to get up to n */
    size_t needed = (size_t)n + 1;
    if (a_cap < needed) {
        size_t new_cap = a_cap * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        a_cache = tmp;
        a_cap = new_cap;
    }
    if (s_cap < needed) {
        size_t new_cap = s_cap * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(s_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        s_cache = tmp;
        s_cap = new_cap;
    }

    /* compute values up to n (if needed) */
    for (size_t i = (size_t)a_max + 1; i <= (size_t)n; ++i) {
        /* sum Σ_{k=0}^{i-1} S(i-1,k) * a(k) */
        seq_t sum = 0;
        for (size_t k = 0; k < i; ++k) {
            sum += s_cache[k] * a_cache[k];
        }

        /* a(i) = 2 * i * sum */
        a_cache[i] = (seq_t)2 * (seq_t)i * sum;

        /* update Stirling numbers to next row i */
        /* new element S(i,i) = 1 */
        s_cache[i] = 1;
        /* compute for k = i-1 ... 1: S(i,k) = k * S(i-1,k) + S(i-1,k-1) */
        for (size_t k = i - 1; k > 0; --k) {
            s_cache[k] = s_cache[k] * (seq_t)k + s_cache[k - 1];
        }
        /* S(i,0) = 0 for i > 0 */
        s_cache[0] = 0;
    }

    a_max = n;
    return a_cache[n];
}
