/*
 * A382237  Numbers that are not divisible by the sum of any subset of their
 *          digits.
 * https://oeis.org/A382237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer that is NOT divisible by the
 *            sum of any non‑empty subset of its decimal digits. The function
 *            generates terms by brute‑force testing each integer, storing
 *            found terms in a static cache so that calls with arbitrary
 *            ordering of n are efficient
 * Range:     u8 n <= 37, u16 n <= 8639, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#ifdef __cplusplus
#include <new>
#endif

/* static cache for memoisation ------------------------------------------------- */
static seq_t *cache = NULL;          /* array of found terms                 */
static idx_t cache_size  = 0;        /* number of cached terms               */
static idx_t cache_cap   = 0;        /* current capacity of the cache        */
static seq_t last_value  = 1;        /* last candidate examined             */

/* ----------------------------------------------------------------------------- *
 *  Expand the cache array to accommodate at least `need` elements.               *
 *  Works both in C (malloc/realloc) and C++ (new/delete).                        */
static void ensure_cache(idx_t need)
{
    if (need <= cache_cap) return;

    idx_t new_cap = cache_cap ? cache_cap << 1 : 128;
    while (new_cap < need) new_cap <<= 1;

#ifdef __cplusplus
    seq_t *new_arr = new seq_t[new_cap];
#else
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
    if (!new_arr) exit(EXIT_FAILURE);

    /* copy existing elements */
    for (idx_t i = 0; i < cache_size; ++i) {
        new_arr[i] = cache[i];
    }

    /* free old storage */
#ifdef __cplusplus
    delete[] cache;
#else
    free(cache);
#endif

    cache     = new_arr;
    cache_cap = new_cap;
}

/* ----------------------------------------------------------------------------- *
 *  Determine whether a candidate `x` passes the test.                            */
static bool is_valid(seq_t x)
{
    /* Decompose digits into an array (least‑significant first). */
    uint8_t digits[256];
    size_t d = 0;
    seq_t tmp = x;
    if (tmp == 0) {                /* 0 is not part of the sequence */
        digits[d++] = 0;
    } else {
        while (tmp) {
            seq_t dgt = tmp % 10;
            digits[d++] = (uint8_t)dgt;
            tmp = tmp / 10;
        }
    }

    /* Compute the maximum possible subset sum. */
    int max_sum = 0;
    for (size_t i = 0; i < d; ++i) max_sum += digits[i];

    /* Dynamic array of reachable sums. */
    bool *reachable = (bool *)calloc(max_sum + 1, sizeof(bool));
    if (!reachable) exit(EXIT_FAILURE);
    reachable[0] = true;

    /* Generate all subset sums. */
    for (size_t i = 0; i < d; ++i) {
        int v = digits[i];
        for (int s = max_sum - v; s >= 0; --s) {
            if (reachable[s]) reachable[s + v] = true;
        }
    }

    /* Check divisibility by any non‑zero reachable sum. */
    for (int s = 1; s <= max_sum; ++s) {
        if (reachable[s] && (x % (seq_t)s == 0)) {
            free(reachable);
            return false;
        }
    }

    free(reachable);
    return true;
}

/* ----------------------------------------------------------------------------- *
 *  Main sequence function.  It generates terms until the n‑th term is found.     */
extern seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safe guard for unused offset 0 */

    /* Initialise cache on first call. */
    if (!cache) {
        cache_cap = 128;
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
#endif
        if (!cache) exit(EXIT_FAILURE);
    }

    /* Generate terms until we have at least n terms. */
    while (cache_size < n) {
        seq_t candidate = last_value + 1;
        last_value = candidate;

        if (is_valid(candidate)) {
            ensure_cache(cache_size + 1);
            cache[cache_size++] = candidate;
        }
    }

    return cache[n - 1];
}
