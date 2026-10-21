/*
 * A291671  Numbers k such that 3 is the smallest decimal digit of k^4.
 * https://oeis.org/A291671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 70, u32 n <= 1006, u64 n <= 1006,
 *            u128 n <= 1006, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A291671: numbers k such that the smallest decimal digit of k^4 is 3. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper: check whether k^4 has smallest decimal digit 3. */
static bool good_k(idx_t k)
{
    /* use 128‑bit arithmetic to avoid overflow for k up to the target size. */
    unsigned __int128 v = (unsigned __int128)k * k;  /* k^2 */
    v *= v;                                         /* k^4 */

    int min_digit = 10;      /* greater than any decimal digit */
    while (v > 0) {
        int d = (int)(v % 10);   /* last decimal digit */
        if (d < min_digit) {
            min_digit = d;
            if (min_digit < 3)   /* cannot be minimal 3 */
                return false;
        }
        v /= 10;
    }
    /* All digits >=3 and at least one 3 (implied by min_digit==3). */
    return min_digit == 3;
}

/* Memoized static cache of sequence values. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next_k = 1;   /* next integer to test */

/* Ensure cache can hold at least 'need' elements. */
static void ensure_cache_size(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed – abort; no further sequence values will be produced. */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Generate sequence values until at least n elements are stored. */
static void generate_up_to(idx_t n)
{
    while (cache_len < (size_t)n) {
        if (good_k(next_k)) {
            ensure_cache_size(cache_len + 1);
            cache[cache_len] = (seq_t)next_k;
            cache_len++;
        }
        next_k++;
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* undefined for n<1, but safe to return 0 */

    /* generate values up to n if necessary */
    generate_up_to(n);

    return cache[n - 1];
}
