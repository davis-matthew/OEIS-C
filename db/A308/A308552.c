/*
 * A308552  a(n) is the least integer such that concat(a(0), a(1), ...,
 *          a(n-1), a(n)) == n-1 (mod n), with a(0) = 0.
 * https://oeis.org/A308552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = least integer a(n) such that concat(a(0), a(1), …,
 *            a(n‑1), a(n)) ≡ n‑1 (mod n) with a(0) = 0. The function builds
 *            the sequence lazily, keeps a cache of already-computed terms,
 *            and performs a brute‑force search for the smallest candidate
 *            that satisfies the congruence
 * Range:     u8 n <= 158, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: allocation is done with malloc()/realloc() and cast to seq_t*,
   which works both for plain integer types in C and for the arbitrary‑
   precision class in the C++ build. */
#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* Static cache of computed terms.  cache_len = number of computed
       entries (i.e. max_index + 1).  cache_cap = allocated size. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* valid entries so far */
    static size_t cache_cap = 0;   /* current allocated capacity */

    /* Ensure capital for index n */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Ensure a(0)=0 is defined */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* Compute terms up to n if necessary */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i)
    {
        if (i == 0) { /* already set above */
            continue;
        }

        /* Compute remainder of concatenation of a[0..i-1] modulo i */
        seq_t r = (seq_t)0;
        seq_t mod = (seq_t)i;

        for (idx_t j = 0; j < i; ++j) {
            seq_t val = cache[j];
            /* number of decimal digits of val */
            seq_t tmp = val;
            unsigned d = 0;
            if (tmp == 0) d = 1;
            else while (tmp) { tmp /= (seq_t)10; ++d; }

            /* 10^d mod i */
            seq_t pow10 = (seq_t)1;
            for (unsigned k = 0; k < d; ++k)
                pow10 = (pow10 * (seq_t)10) % mod;

            r = (r * pow10 + val) % mod;
        }

        /* Find smallest x such that (r * 10^{len(x)} + x) % i == i-1 */
        seq_t x = (seq_t)0;
        seq_t target = (seq_t)((i - 1) % i);
        for (;;) {
            seq_t tmp = x;
            unsigned d = 0;
            if (tmp == 0) d = 1;
            else while (tmp) { tmp /= (seq_t)10; ++d; }

            seq_t pow10 = (seq_t)1;
            for (unsigned k = 0; k < d; ++k)
                pow10 = (pow10 * (seq_t)10) % mod;

            seq_t new_r = (r * pow10 + x) % mod;
            if (new_r == target) {
                cache[i] = x;
                break;
            }
            ++x;
        }
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
