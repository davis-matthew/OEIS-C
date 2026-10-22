/*
 * A336830  The Sydney Opera House sequence: a(0) = 0, a(1) = 1; for n > 0,
 *          a(n) = min(a(n-1)/n if n|a(n-1), a(n-1)-n) where a(n) is
 *          nonnegative and not already in the sequence. Otherwise a(n) =
 *          min(a(n-1)+n, a(n-1)*n) where a(n) is not already in the sequence.
 *          Otherwise a(n) = a(n-1) + n.
 * https://oeis.org/A336830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence definition of OEIS A336830 * a(0) = 0, a(1) = 1 * For
 *            n > 0: * let prev = a(n-1) * candidates1 = { prev - n (if prev
 *            >= n), prev / n (if n | prev) } * if any candidate in
 *            candidates1 has not appeared before, a(n) = min of those * else
 *            * candidates2 = { prev + n, prev * n } * if any can...
 * Range:     u8 n <= 29, u16 n <= 346, u32 n <= 8329, u64 n <= 8329,
 *            u128 n <= 8329, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache for computed sequence values */
static seq_t *seq_cache = NULL;      /* array of seq_t values */
static size_t cache_len = 0;         /* number of elements stored */
static size_t cache_cap = 0;         /* allocated capacity */

/* Ensure that seq_cache has at least n+1 elements (indices 0..n) */
static void ensure_cache_upto(idx_t n)
{
    if (cache_len > n) return;
    /* Extend capacity if needed */
    if (cache_cap < (size_t)(n + 1))
    {
        size_t new_cap = cache_cap ? cache_cap : 128;
        while (new_cap <= (size_t)(n + 1)) new_cap <<= 1;
        seq_t *new_arr = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_arr) abort(); /* memory allocation failure */
        seq_cache = new_arr;
        cache_cap = new_cap;
    }
    /* Initialize new elements */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)  seq_cache[i] = (seq_t)0;
        else if (i == 1) seq_cache[i] = (seq_t)1;
        else
        {
            seq_t prev = seq_cache[i - 1];
            seq_t cand[4];
            bool cand_ok[4];
            int cand_cnt = 0;

            /* Candidate prev - n if valid */
            if (prev >= (seq_t)i)
            {
                cand[cand_cnt] = prev - (seq_t)i;
                cand_ok[cand_cnt] = true;
                ++cand_cnt;
            }
            /* Candidate prev / n if n divides prev */
            if (prev % (seq_t)i == (seq_t)0)
            {
                cand[cand_cnt] = prev / (seq_t)i;
                cand_ok[cand_cnt] = true;
                ++cand_cnt;
            }

            /* Find first available candidate from step 1 */
            seq_t chosen = 0;
            bool found = false;
            for (int j = 0; j < cand_cnt; ++j)
            {
                seq_t v = cand[j];
                /* Check if v already in sequence */
                bool seen = false;
                for (idx_t k = 0; k < i; ++k)
                    if (seq_cache[k] == v) { seen = true; break; }
                if (!seen)
                {
                    if (!found || v < chosen) chosen = v, found = true;
                }
            }

            if (!found)
            {
                /* Step 2 candidates: prev + n , prev * n */
                seq_t add = prev + (seq_t)i;
                seq_t mul = prev * (seq_t)i;
                seq_t minv = 0;
                bool has_avail = false;
                /* Check add */
                bool already = false;
                for (idx_t k = 0; k < i; ++k)
                    if (seq_cache[k] == add) { already = true; break; }
                if (!already) { minv = add; has_avail = true; }

                /* Check mul */
                already = false;
                for (idx_t k = 0; k < i; ++k)
                    if (seq_cache[k] == mul) { already = true; break; }
                if (!already)
                {
                    if (!has_avail || mul < minv) { minv = mul; has_avail = true; }
                }

                if (has_avail) chosen = minv;
                else chosen = add; /* fallback to prev + n */
            }

            seq_cache[i] = chosen;
        }
    }
    cache_len = (size_t)(n + 1);
}

/* Public function implementing the sequence */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;
    ensure_cache_upto(n);
    return seq_cache[n];
}
