/*
 * A286103  a(1) = 0; for n > 1, a(n) = 1 + min(a(A285734(n)), a(A285735(n))).
 * https://oeis.org/A286103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n > 1, a(n) = 1 + min(a(A285734(n)),
 *            a(A285735(n)))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where A285734(n) is the largest square‑free integer ≤ floor(n/2) such that both
   it and n – that integer are square‑free.  A285735(n) is its complement n – A285734(n). */

/* Implementation uses memoisation.  For n up to 10,000 the recursion depth and
   pair search are tiny.  A simple square‑free test is used. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* local cache for memoised a(n) values; index 0 is unused, a(1)=0 */
static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;

/* Ensure cache has room for indices up to 'n' */
static void ensure_a_cache(idx_t n)
{
    if (n < a_cache_cap)
        return;
    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 1;
    while (new_cap <= n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    /* initialise new entries to 0 (uncomputed) */
    for (size_t i = a_cache_cap; i < new_cap; ++i)
        tmp[i] = 0;
    a_cache = tmp;
    a_cache_cap = new_cap;
}

/* Test whether x is square‑free (for 64‑bit values; n <= 10 000). */
static inline bool is_squarefree_uint64(uint64_t x)
{
    if (x <= 1)
        return true;
    uint64_t n = x;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int cnt = 0;
            while (n % p == 0) {
                n /= p;
                ++cnt;
                if (cnt > 1)
                    return false;
            }
        }
    }
    /* Remaining factor is > sqrt(original) and has exponent 1 */
    return true;
}

/* Recursive routine to compute a(n) with memoisation */
static seq_t compute_a(idx_t n)
{
    if (n == 1)
        return 0;

    ensure_a_cache(n);
    if (a_cache[n] != 0)                /* already computed */
        return a_cache[n];

    /* Find largest square‑free L <= floor(n/2) with n-L also square‑free */
    idx_t half = n / 2;
    idx_t L;
    for (L = half; L >= 1; --L) {
        if (is_squarefree_uint64((uint64_t)L) &&
            is_squarefree_uint64((uint64_t)(n - L)))
            break;
    }
    idx_t R = n - L;

    seq_t left_val  = (L == 1) ? 0 : compute_a(L);
    seq_t right_val = (R == 1) ? 0 : compute_a(R);

    seq_t res = 1 + (left_val < right_val ? left_val : right_val);
    a_cache[n] = res;
    return res;
}

/* External linked function required by the build system */
seq_t a(idx_t n)
{
    return compute_a(n);
}
