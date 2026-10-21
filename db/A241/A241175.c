/*
 * A241175  Numbers which cannot be obtained by adding some digit of a number
 *          m to m.
 * https://oeis.org/A241175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A241175: numbers that cannot be written as m + d where d is a digit of m */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the build environment. */
/* ------------------------------------------------------------------ */
/* Helper: does `x` contain the decimal digit `d`?                      */
/* ------------------------------------------------------------------ */
static bool has_digit(seq_t x, unsigned d)
{
    if (x == 0)
        return d == 0;
    while (x > 0) {
        if ((x % 10) == d)
            return true;
        x /= 10;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Helper: is `k` expressible as m + d with d a digit of m?            */
/* ------------------------------------------------------------------ */
static bool is_expressible(seq_t k)
{
    for (unsigned d = 0; d < 10; ++d) {
        if (k < d)
            continue;           /* would underflow */
        seq_t m = k - d;
        if (has_digit(m, d))
            return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Cache infrastructure                                                */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;          /* array of found terms */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* allocated capacity */
static seq_t next_candidate = 1;     /* next integer to test */

/* Resize the cache if needed */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                     /* unrecoverable error */
    cache = tmp;
    cache_cap = new_cap;
}

/* Ensure that the cache contains at least `n` terms (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;                      /* already enough terms */
    while (cache_len < n) {
        if (!is_expressible(next_candidate)) {
            if (cache_len == cache_cap)
                grow_cache();
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
}

/* ------------------------------------------------------------------ */
/* Public API: a(idx_t n)                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* n is 1‑based; behaviour for n==0 is undefined. */
    ensure_cache(n);
    return cache[n - 1];
}
