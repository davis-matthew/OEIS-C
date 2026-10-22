/*
 * A370888  a(n) = (n-1)*(2*(n-2)!+1).
 * https://oeis.org/A370888
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1) * ( 2 * (n-2)! + 1 )
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 2  (a(2) is the first term)  */

#include <stddef.h>

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> fact_cache(1, (seq_t)1);    /* fact_cache[0] = 1 */

static inline void ensure_factorial(idx_t m)
{
    if (fact_cache.size() > m) return;
    fact_cache.reserve(m + 1);
    for (idx_t i = fact_cache.size(); i <= m; ++i) {
        fact_cache.push_back(fact_cache.back() * (seq_t)i);
    }
}
#else
#include <stdlib.h>
static seq_t *fact_cache = NULL;      /* will hold factorials up to the largest requested n-2  */
static size_t fact_cap   = 0;         /* allocation capacity  */
static idx_t fact_max    = 0;         /* highest computed factorial index */

static inline void ensure_factorial(idx_t m)
{
    if (fact_cache == NULL) {                     /* first time: init to 0! = 1 */
        fact_cap = 1;
        fact_cache = (seq_t *)malloc(sizeof(seq_t));
        fact_cache[0] = (seq_t)1;
        fact_max = 0;
    }
    if (fact_max >= m) return;

    size_t needed = m + 1;
    if (fact_cap < needed) {                      /* grow array to fit m+1 entries */
        size_t new_cap = fact_cap ? fact_cap : 1;
        while (new_cap < needed) new_cap <<= 1;
        fact_cache = (seq_t *)realloc(fact_cache, sizeof(seq_t) * new_cap);
        fact_cap = new_cap;
    }

    for (idx_t i = fact_max + 1; i <= m; ++i) {   /* compute missing factorials */
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }
    fact_max = m;
}
#endif

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* value undefined for n < 2, but caller may ignore */

    idx_t m = n - 2;           /* we need (n-2)!  */
    ensure_factorial(m);

    seq_t f = fact_cache[m];        /* (n-2)! */
    seq_t term = f * (seq_t)2 + (seq_t)1;          /* 2*(n-2)! + 1 */
    seq_t result = ((seq_t)(n - 1)) * term;        /* (n-1)*(2*(n-2)!+1) */
    return result;
}
