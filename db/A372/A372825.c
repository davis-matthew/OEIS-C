/*
 * A372825  Exponents of 3 and 2 in sequence A372823.
 * https://oeis.org/A372825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(2n) = n,
 *          a(2n+1) = least k such that 3^n <= 2^k
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp); */
#include "../utils/pow3.h"   /* provides seq_t pow3(idx_t exp); */

/* cache for computed powers of 3 (3^i) */
static seq_t *pow3_cache = NULL;      /* 3^i stored at pow3_cache[i] */
static size_t pow3_cap   = 0;         /* current size of pow3_cache */

/* cache for the sequence values a(i) */
static seq_t *seq_cache   = NULL;     /* a(i) stored at seq_cache[i] */
static idx_t  seq_cache_len = 0;      /* number of initialized entries */

/* extend pow3_cache to include 3^m */
static void ensure_pow3(idx_t m)
{
    if (m >= (idx_t)pow3_cap) {
        /* grow the cache */
        size_t new_cap = pow3_cap == 0 ? 1 : pow3_cap;
        while ((idx_t)new_cap <= m) new_cap *= 2;
        pow3_cache = (seq_t *)realloc(pow3_cache, new_cap * sizeof(seq_t));
        if (pow3_cache == NULL) abort();          /* allocation failed */
        if (pow3_cap == 0) {
            pow3_cache[0] = (seq_t)1;
            pow3_cap = 1;
        }
        for (size_t i = pow3_cap; i <= (size_t)m; ++i)
            pow3_cache[i] = pow3_cache[i - 1] * (seq_t)3;
        pow3_cap = (size_t)m + 1;
    }
}

/* binary search for minimal k with 2^k >= target */
static seq_t find_k_for_pow3(idx_t m)
{
    const seq_t target = pow3_cache[m];   /* 3^m */

    /* Upper bound: 2^(2*m) >= 3^m for m >= 0 */
    idx_t low = 0;
    idx_t high = 2 * m;                  /* high is inclusive for safety */

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (pow2(mid) >= target)
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* enlarge the sequence cache if needed */
    if (n >= seq_cache_len) {
        size_t new_len = n + 1;
        seq_cache = (seq_t *)realloc(seq_cache, new_len * sizeof(seq_t));
        if (seq_cache == NULL) abort();   /* allocation failed */

        for (idx_t i = seq_cache_len; i <= n; ++i) {
            seq_t val;
            if ((i & 1ULL) == 0ULL) {             /* even i : a(2m) = m */
                val = (seq_t)(i >> 1);
            } else {                             /* odd i : a(2m+1) = k */
                idx_t m = (i - 1) >> 1;
                ensure_pow3(m);
                val = find_k_for_pow3(m);
            }
            seq_cache[i] = val;
        }
        seq_cache_len = (idx_t)new_len;
    }
    return seq_cache[n];
}
