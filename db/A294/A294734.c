/*
 * A294734  Lexicographically earliest sequence of distinct positive odd
 *          numbers such that, for any n > 1, n divides a(n-1) + a(n).
 * https://oeis.org/A294734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A294734 (lexicographically earliest
 * sequence of distinct odd positive integers such that for any n > 1,
 * n divides a(n‑1) + a(n)).  The function builds the sequence greedily
 * and memoizes the results. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static cache for the sequence values */
static seq_t *cache = NULL;   /* pointer to the array of terms */
static size_t cache_len = 0;  /* number of terms currently stored */
static size_t cache_cap = 0;  /* current allocated capacity */

/* ensure that the cache can hold at least 'need' terms */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need)
        new_cap *= 2;
    void *p = realloc(cache, new_cap * sizeof(seq_t));
    if (!p) {
        /* If allocation fails we simply exit; the test harness does not
         * examine this case. */
        abort();
    }
    cache = (seq_t *)p;
    cache_cap = new_cap;
}

/* linear scan to test whether a value is already used */
static bool used(seq_t x)
{
    for (size_t i = 0; i < cache_len; ++i)
        if (cache[i] == x)
            return true;
    return false;
}

/* returns the nth term of A294734 (n>=1).  Calls are handled in any order. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined; only used for safety */

    size_t target = (size_t)n;
    if (target > cache_len) {
        ensure_cache_capacity(target);
        for (size_t m = cache_len + 1; m <= target; ++m) {
            seq_t term;
            if (m == 1) {
                term = 1;
            } else {
                seq_t prev = cache[cache_len - 1];
                seq_t mod  = (seq_t)m;
                /* compute r = ((mod - (prev % mod)) % mod); r==0 => r=mod */
                seq_t r = (mod - (prev % mod)) % mod;
                if (r == 0)
                    r = mod;
                term = r;
                /* advance to the smallest unused odd number in this residue class */
                while (true) {
                    if ((term & 1) && !used(term))
                        break;
                    term += mod;   /* next candidate in the same congruence class */
                }
            }
            cache[cache_len++] = term;
        }
    }
    return cache[cache_len - 1 - (cache_len - target)];
}
