/*
 * A373261  Numbers k for which A276085(k) == +1 (mod 3), where A276085 is the
 *          primorial base log-function.
 * https://oeis.org/A373261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sequence of integers k such that v2(k)-v3(k) ≡ 1 (mod 3). The
 *            function searches numbers in increasing order and returns the
 *            n‑th term. No small lookup tables are used so the implementation
 *            works for all n ≥ 1, regardless of call order
 * Range:     u8 n <= 69, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* --------------------------------------------------------------------- */
/* Helpers:  v2(k) = exponent of 2 in k,  v3(k) = exponent of 3 in k */

static bool good(idx_t k)
{
    int e2 = 0;
    idx_t t = k;
    while ((t & 1) == 0) {
        e2++;
        t >>= 1;
    }
    int e3 = 0;
    while (t % 3 == 0) {
        e3++;
        t /= 3;
    }
    int mod = ((e2 - e3) % 3 + 3) % 3;
    return mod == 1;
}

/* --------------------------------------------------------------------- */
/* Storage for the generated terms */

static size_t len   = 0;          /* number of terms stored so far */
static idx_t  cur_k = 1;          /* next integer to test (k starts at 2) */

#ifndef __cplusplus
static seq_t *cache = NULL;
static size_t cap   = 0;
#endif

#ifdef __cplusplus
static std::vector<seq_t> cache_vec;
#endif

/* --------------------------------------------------------------------- */
/* Expand the cache until it contains at least n terms (1‑based index) */

static void expand(idx_t n)
{
    while (len < (size_t)n) {
        cur_k++;
        if (good(cur_k)) {
#ifdef __cplusplus
            cache_vec.push_back((seq_t)cur_k);
            ++len;
#else
            if (len >= cap) {
                size_t new_cap = cap ? cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* allocation failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = (seq_t)cur_k;
#endif
        }
    }
}

/* --------------------------------------------------------------------- */
/*  a(n) function required by the build.  n ≥ 1.  Undefined for n==0.     */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safe fallback; OEIS offset is 1 */
    if (len < (size_t)n) expand(n);
    return
#ifdef __cplusplus
        cache_vec[n - 1];
#else
        cache[n - 1];
#endif
}
