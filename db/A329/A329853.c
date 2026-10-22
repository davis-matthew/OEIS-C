/*
 * A329853  Numbers having twice as many terms in their Zeckendorf expansion
 *          as 1's in their binary expansion.
 * https://oeis.org/A329853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 32, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A329853: numbers k whose Zeckendorf expansion contains
   twice as many terms as the number of 1's in the binary expansion. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <vector>
#endif

/* count number of 1‑bits of a seq_t value */
static size_t popcount_seq(seq_t x)
{
    size_t cnt = 0;
    while (x)
    {
        cnt += (size_t)(x & 1);
        x >>= 1;
    }
    return cnt;
}

/* greedy Zeckendorf decomposition: returns the number of terms */
static int zeckendorf_terms(seq_t x, const seq_t *fib, size_t fib_len)
{
    int terms = 0;
    while (x)
    {
        /* find largest Fibonacci <= x */
        size_t i = fib_len;
        while (i > 0 && fib[i - 1] > x)
            --i;
        x -= fib[i - 1];
        ++terms;
    }
    return terms;
}

/* Return a(n) = the nth integer of A329853 (1‑based). */
seq_t a(idx_t n)
{
    /* static state for cached terms */
#ifdef __cplusplus
    static std::vector<seq_t> cacheVec;
    static std::vector<seq_t> fibVec;
#else
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t *fib = NULL;
    static size_t fib_len = 0;
    static size_t fib_cap = 0;
#endif

    /* static state for candidate generation */
    static seq_t last_checked = 0;

    /*--- initialize Fibonacci table ---*/
#ifdef __cplusplus
    if (fibVec.empty())
    {
        fibVec.push_back((seq_t)1); /* F1 = 1   */
        fibVec.push_back((seq_t)2); /* F2 = 2   */
    }
#else
    if (fib_len == 0)
    {
        fib_cap = 64;
        fib = (seq_t *)malloc(fib_cap * sizeof(seq_t));
        fib[0] = (seq_t)1;
        fib[1] = (seq_t)2;
        fib_len = 2;
    }
#endif

    /*--- search until we have found n terms ---*/
    idx_t found = 0;
    while (found < n)
    {
        ++last_checked;

        /* ensure Fibonacci array covers last_checked */
#ifdef __cplusplus
        while (fibVec.back() < last_checked)
            fibVec.push_back(fibVec[fibVec.size() - 1] + fibVec[fibVec.size() - 2]);
        const seq_t *fib_ptr = fibVec.data();
        size_t fib_size = fibVec.size();
#else
        while (fib[fib_len - 1] < last_checked)
        {
            if (fib_len == fib_cap)
            {
                fib_cap *= 2;
                fib = (seq_t *)realloc(fib, fib_cap * sizeof(seq_t));
            }
            fib[fib_len] = fib[fib_len - 1] + fib[fib_len - 2];
            ++fib_len;
        }
        const seq_t *fib_ptr = fib;
        size_t fib_size = fib_len;
#endif

        /* test candidate */
        int zterms = zeckendorf_terms(last_checked, fib_ptr, fib_size);
        size_t ones  = popcount_seq(last_checked);

        if ((size_t)zterms == 2 * ones)
        {
#ifdef __cplusplus
            cacheVec.push_back(last_checked);
#else
            if (cache_len == cache_cap)
            {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = last_checked;
#endif
            ++found;
        }
    }

    /*--- return the nth term ---*/
#ifdef __cplusplus
    return cacheVec[(size_t)n - 1];
#else
    return cache[(size_t)n - 1];
#endif
}
