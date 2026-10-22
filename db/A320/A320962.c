/*
 * A320962  a(n) = (-1)^(n-1)*(n-1)!*Sum_{i=0..n} Stirling2(n, i) if n > 0 and
 *          0 otherwise.
 * https://oeis.org/A320962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, changed
 * Formula:   a(n) = (-1)^(n+1) * B(n) * (n-1)! for n>=1, a(0)=0
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 14, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#endif

/* ---------- C++ implementation (uses std::vector) ---------- */
#ifdef __cplusplus
static std::vector<seq_t> bell_cache{ seq_t(1) };   /* B(0) = 1 */
static std::vector<seq_t> fact_cache{ seq_t(1) };   /* (0)! = 1 */

static void ensure_bell_cpp(idx_t n)
{
    if (bell_cache.size() > n) return;
    size_t cur = bell_cache.size();
    bell_cache.resize(n + 1);                /* default slots zero-initialised */

    for (size_t m = cur; m <= n; ++m) {
        seq_t sum  = (seq_t)0;
        seq_t binom= (seq_t)1;                 /* C(m-1,0) = 1 */
        for (size_t k = 0; k < m; ++k) {
            sum += binom * bell_cache[k];
            if (k + 1 < m)
                binom = (binom * (seq_t)(m - 1 - k)) / (seq_t)(k + 1);
        }
        bell_cache[m] = sum;
    }
}

static void ensure_fact_cpp(idx_t n)
{
    if (fact_cache.size() > n) return;
    size_t cur = fact_cache.size();
    fact_cache.resize(n + 1);
    for (size_t i = cur; i <= n; ++i)
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
}
#endif

/* ---------- C implementation (uses malloc) ---------- */
#ifndef __cplusplus
static seq_t *bell_cache = NULL;
static size_t bell_cache_size = 0;            /* number of stored B(k) */

static seq_t *fact_cache = NULL;
static size_t fact_cache_size = 0;            /* number of stored (k)! */

static void ensure_bell_c(idx_t n)
{
    if (bell_cache_size > n) return;

    seq_t *new_buf = realloc(bell_cache, (n + 1) * sizeof(seq_t));
    if (!new_buf) exit(EXIT_FAILURE);
    bell_cache = new_buf;

    if (bell_cache_size == 0) {
        bell_cache[0] = (seq_t)1;          /* B(0) = 1 */
        bell_cache_size = 1;
    }

    for (size_t m = bell_cache_size; m <= n; ++m) {
        seq_t sum  = (seq_t)0;
        seq_t binom= (seq_t)1;             /* C(m-1,0) = 1 */
        for (size_t k = 0; k < m; ++k) {
            sum += binom * bell_cache[k];
            if (k + 1 < m)
                binom = (binom * (seq_t)(m - 1 - k)) / (seq_t)(k + 1);
        }
        bell_cache[m] = sum;
    }
    bell_cache_size = n + 1;
}

static void ensure_fact_c(idx_t n)
{
    if (fact_cache_size > n) return;

    seq_t *new_buf = realloc(fact_cache, (n + 1) * sizeof(seq_t));
    if (!new_buf) exit(EXIT_FAILURE);
    fact_cache = new_buf;

    if (fact_cache_size == 0) {
        fact_cache[0] = (seq_t)1;          /* 0! = 1 */
        fact_cache_size = 1;
    }

    for (size_t i = fact_cache_size; i <= n; ++i)
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;

    fact_cache_size = n + 1;
}
#endif

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

#ifdef __cplusplus
    ensure_bell_cpp(n);
    ensure_fact_cpp(n - 1);
    seq_t fact  = fact_cache[n - 1];
    seq_t bell  = bell_cache[n];
#else
    ensure_bell_c(n);
    ensure_fact_c(n - 1);
    seq_t fact  = fact_cache[n - 1];
    seq_t bell  = bell_cache[n];
#endif

    seq_t result = fact * bell;
    /* sign = (-1)^(n+1) ; n even => sign negative */
    if ((n & 1) == 0)
        result = -result;

    return result;
}
