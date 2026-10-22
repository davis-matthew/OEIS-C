/*
 * A394758  A sequence constructed by greedily sampling the Hermite
 *          distribution, with both parameters as 1, to minimize discrepancy.
 * https://oeis.org/A394758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is obtained by greedy selection from the Hermite
 *            distribution with parameters 1,1. For each step n (starting at
 *            1) we compute f(k) = n*p(k) – c(k) where p(k) is the probability
 *            mass for k and c(k) is the number of times k has already
 *            appeared. The term chosen for a(n) is the k with maximal f...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define KMAX 2000

static bool init_done = false;
static long double *ps = NULL;          /* p(k) */
static long double *fact = NULL;        /* factorials for probability computation */
static unsigned long long *counts = NULL; /* occurrences c(k) */
static seq_t *memo = NULL;              /* memoised a(n) values */
static size_t memo_len = 0;             /* number of stored terms (a(1) at memo[0]) */
static size_t memo_cap = 0;             /* current allocation size */

static void init_seq(void)
{
    if (init_done) return;

    ps     = (long double *)malloc((KMAX + 1) * sizeof(long double));
    fact   = (long double *)malloc((KMAX + 1) * sizeof(long double));
    counts = (unsigned long long *)calloc((KMAX + 1), sizeof(unsigned long long));

    fact[0] = 1.0L;
    for (size_t i = 1; i <= (size_t)KMAX; ++i)
        fact[i] = fact[i-1] * (long double)i;

    const long double exp2 = expl(2.0L);   /* e² */
    for (size_t k = 0; k <= (size_t)KMAX; ++k) {
        long double sum = 0.0L;
        size_t lim = k / 2;
        for (size_t i = 0; i <= lim; ++i) {
            size_t m = k - 2*i;
            sum += 1.0L / (fact[i] * fact[m]);
        }
        ps[k] = sum / exp2;
    }

    init_done = true;
}

static void ensure_seq_capacity(size_t needed)
{
    if (needed <= memo_cap) return;
    size_t new_cap = memo_cap ? memo_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_mem = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!new_mem) exit(EXIT_FAILURE);
    memo = new_mem;
    memo_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset 1; n=0 is undefined */

    init_seq();

    if ((size_t)n <= memo_len) return memo[n-1];

    ensure_seq_capacity((size_t)n);

    for (idx_t cur = memo_len + 1; cur <= n; ++cur) {
        long double best = -1e300L;
        size_t bestk = 0;
        for (size_t k = 0; k <= (size_t)KMAX; ++k) {
            long double val = (long double)cur * ps[k] - (long double)counts[k];
            if (val > best || (fabsl(val - best) <= 1e-18L && k < bestk)) {
                best = val;
                bestk = k;
            }
        }
        memo[memo_len] = (seq_t)bestk;
        counts[bestk] += 1;
        ++memo_len;
    }
    return memo[n-1];
}
