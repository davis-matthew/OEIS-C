/*
 * A355372  Expansion of the e.g.f. log((1 - x) / (1 - 2*x)) / (1 - x)^3.
 * https://oeis.org/A355372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A355372
   Formula: a(n) = 3*n*a(n-1) - 2*(n^2-1)*a(n-2) + (n+1)!/2   (n > 1)
   with a(0) = 0, a(1) = 1.
   Uses memoisation: a(idx_t n) works for any n ≥ 0, in any order. */
 
#include <stddef.h>
#include <stdlib.h>

/* static storage for values of the sequence (cache) */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* allocated size of cache   */
static idx_t cache_up_to = 0;   /* highest n whose a(n) is known */

/* static storage for factorials */
static seq_t *fact = NULL;
static size_t fact_size = 0;    /* allocated size of factorial array   */
static idx_t fact_up_to = 0;    /* highest k whose k! is known */

/* enlarge factorial array up to 'up_to' (inclusive) and compute missing factorials */
static void ensure_factorial(idx_t up_to)
{
    if (fact == NULL) {
        fact_size = up_to + 1;
        if (fact_size == 0) fact_size = 1;
        fact = (seq_t*)malloc(fact_size * sizeof(seq_t));
        fact[0] = 1;          /* 0! = 1 */
        fact_up_to = 0;
    } else if (fact_size <= up_to) {
        size_t new_size = up_to + 1;
        seq_t *tmp = (seq_t*)realloc(fact, new_size * sizeof(seq_t));
        fact = tmp;
        fact_size = new_size;
    }
    for (idx_t i = fact_up_to + 1; i <= up_to; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }
    fact_up_to = up_to;
}

/* enlarge cache array up to 'up_to' (inclusive) and allocate space (values computed later) */
static void ensure_cache(idx_t up_to)
{
    if (cache == NULL) {
        cache_size = up_to + 1;
        if (cache_size == 0) cache_size = 1;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        cache[0] = 0;          /* a(0) = 0 */
        cache_up_to = 0;
    } else if (cache_size <= up_to) {
        size_t new_size = up_to + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;
        cache_size = new_size;
    }
    /* no value calculation here – done in a() */
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;   /* a(1) is always 1 */

    /* ensure tables are large enough */
    ensure_cache(n);
    ensure_factorial(n + 1);   /* we need (n+1)! */

    /* compute any missing values up to n */
    for (idx_t i = cache_up_to + 1; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 0;
            continue;
        }
        if (i == 1) {
            cache[1] = 1;
            continue;
        }
        seq_t seq_i   = (seq_t)i;
        seq_t term1   = 3 * seq_i * cache[i - 1];
        seq_t nsq     = seq_i * seq_i;
        seq_t term2   = 2 * (nsq - 1) * cache[i - 2];
        seq_t fact_it = fact[i + 1] / 2;   /* (i+1)! / 2 */
        cache[i] = term1 - term2 + fact_it;
    }
    cache_up_to = n;
    return cache[n];
}
