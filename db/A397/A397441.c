/*
 * A397441  a(n) is the number of partitions of n into parts of the form
 *          2^r*(2^s - 1) with each r distinct.
 * https://oeis.org/A397441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(2*n)=a(n); a(2*n+1)=Σ_{i=0..⌊log₂(n+1)⌋} a(n-(2^i-1))
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache: grows to the largest n requested so far */
    static seq_t *cache = NULL;
    static idx_t max_n = 0;
    static size_t cap   = 0;

    /* initialise on first call */
    if (cache == NULL) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out‑of‑memory: caller will receive 0 */
        cache[0] = (seq_t)1;                  /* a(0) = 1 */
        max_n = 0;
    }

    if (n > max_n) {
        /* enlarge cache if necessary */
        size_t needed = (size_t)n + 1;
        if (needed > cap) {
            size_t new_cap = cap ? cap << 1 : 1;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;        /* out‑of‑memory */
            if (new_cap > cap)
                /* initialise the newly allocated part to zero */
                memset((void *)(tmp + cap), 0, (new_cap - cap) * sizeof(seq_t));
            cache = tmp;
            cap = new_cap;
        }

        /* compute all values up to n */
        for (idx_t k = max_n + 1; k <= n; ++k) {
            if ((k & (idx_t)1ULL) == 0ULL) {      /* even k */
                cache[k] = cache[k >> 1];
            } else {                              /* odd k = 2*m+1 */
                idx_t m   = (k - 1ULL) >> 1;
                seq_t sum = (seq_t)0;
                idx_t pow = 1;                    /* 2^i */
                while (1) {
                    idx_t sub = pow - 1;
                    if (sub > m) break;
                    sum += cache[m - sub];
                    if (pow > (UINT64_MAX >> 1)) break; /* avoid overflow */
                    pow <<= 1;
                }
                cache[k] = sum;
            }
        }
        max_n = n;
    }

    return cache[n];
}
