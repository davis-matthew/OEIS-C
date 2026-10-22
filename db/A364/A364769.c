/*
 * A364769  Numbers k for which k and the arithmetic derivative k' (A003415)
 *          are practical numbers (A005153).
 * https://oeis.org/A364769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A364769: numbers k such that k and its arithmetic derivative k'
   (A003415) are both practical numbers (A005153).  The implementation
   generates the sequence on demand, caching each term as it is found. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cached = NULL;      /* array of found terms */
static size_t cached_len = 0;     /* number of cached terms */
static size_t cached_cap = 0;     /* capacity of the array */

/* Test whether a uint64_t is a practical number. */
static bool is_practical_uint64(uint64_t n)
{
    if (n == 1) return true;
    if ((n & 1U) == 0) {
        int e2 = 0;
        while ((n & 1U) == 0) { n >>= 1U; ++e2; }
        unsigned __int128 sigma = ((unsigned __int128)1 << (e2 + 1)) - 1;
        unsigned __int128 prev = sigma;
        uint64_t rem = n;
        for (uint64_t p = 3; p * p <= rem; p += 2) {
            if (rem % p == 0) {
                int e = 0;
                while (rem % p == 0) { rem /= p; ++e; }
                if ((unsigned __int128)p > prev + 1) return false;
                unsigned __int128 pp = 1;
                for (int i = 0; i < e + 1; ++i) pp *= p;
                unsigned __int128 sp = (pp - 1) / (p - 1);
                prev *= sp;
            }
        }
        if (rem > 1) {
            uint64_t p = rem;
            if ((unsigned __int128)p > prev + 1) return false;
        }
        return true;
    }
    return false;
}

/* Compute the arithmetic derivative of a uint64_t. */
static uint64_t derivative_uint64(uint64_t n)
{
    unsigned __int128 d = 0;
    uint64_t m = n;
    if ((m & 1U) == 0) {
        int e = 0;
        while ((m & 1U) == 0) { m >>= 1U; ++e; }
        d += (unsigned __int128)e * n / 2;
    }
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            d += (unsigned __int128)e * n / p;
        }
    }
    if (m > 1) d += (unsigned __int128)1 * n / m;
    return (uint64_t)d;
}

/* Ensure the cache can hold at least 'cap' elements.  The array is
   reallocated with doubled capacity when needed.  The implementation
   differs between C and C++. */
static void ensure_cache_capacity(size_t cap)
{
    if (cached_cap < cap) {
        size_t new_cap = cached_cap ? cached_cap * 2 : 8;
        if (new_cap < cap) new_cap = cap;
        seq_t *new_arr;
#ifdef __cplusplus
        new_arr = new seq_t[new_cap];
#else
        new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
        if (!new_arr) return;            /* allocation failed – leave as is */
        for (size_t i = 0; i < cached_len; ++i) new_arr[i] = cached[i];
#ifdef __cplusplus
        delete [] cached;
#else
        free(cached);
#endif
        cached = new_arr;
        cached_cap = new_cap;
    }
}

/* Return the nth element of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined – return 0 */
    idx_t target = n - 1;                 /* zero‑based index */
    /* Generate terms until we have enough cached. */
    static uint64_t next_k = 2;           /* the next integer to test */
    while (cached_len <= (size_t)target) {
        uint64_t k = next_k++;
        if (!is_practical_uint64(k)) continue;
        uint64_t d = derivative_uint64(k);
        if (!is_practical_uint64(d)) continue;
        ensure_cache_capacity(cached_len + 1);
        cached[cached_len++] = (seq_t)k;
    }
    return cached[(size_t)target];
}
