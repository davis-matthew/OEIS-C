/*
 * A348487  Positive numbers whose square starts and ends with exactly one 1.
 * https://oeis.org/A348487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by brute‑force search: produce all positive integers
   whose square starts and ends with exactly one digit '1'.
   The sequence is indexed with a(1)=1. */

#include <stdlib.h>
#include <stdbool.h>

/* Small helper: 10^exp, exp fits in 64‑bit and result fits in seq_t */
static inline seq_t pow10_small(idx_t exp)
{
    seq_t r = 1;
    while (exp--) r *= 10;
    return r;
}

/* Test whether the square of x starts and ends with exactly one 1.
   First digit is 1 and second digit (if any) is not 1.
   Last digit is 1 and second last digit (if any) is not 1. */
static inline bool passes(seq_t x)
{
    seq_t sq = x * x;

    /* Last digit must be 1 */
    if ((sq % 10) != 1) return false;

    /* Second last digit must not be 1 (if exists) */
    if (((sq / 10) % 10) == 1) return false;

    /* Count decimal digits of the square */
    idx_t len = 0;
    seq_t tmp = sq;
    while (tmp > 0) {
        tmp /= 10;
        ++len;
    }
    if (len == 0) return false;          /* impossible for positive sq */

    /* First digit */
    seq_t pow10_len_minus1 = pow10_small(len - 1);
    seq_t first = sq / pow10_len_minus1;
    if (first != 1) return false;

    /* Second digit (if length >= 2) */
    if (len >= 2) {
        seq_t pow10_len_minus2 = pow10_len_minus1 / 10;
        seq_t second = (sq / pow10_len_minus2) % 10;
        if (second == 1) return false;
    }

    return true;
}

/* ---------- Cache for computed terms ---------- */
static seq_t *cache = NULL;       /* array of already found terms */
static size_t cache_cap = 0;      /* allocated capacity of cache */
static size_t cache_len = 0;      /* number of terms currently stored */
static seq_t next_candidate = 1;  /* next integer to test */

/* Ensure the cache can hold at least 'need' elements */
static void ensure_cap(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!p) abort();           /* out of memory */
        cache = p;
        cache_cap = new_cap;
    }
}

/* Generate terms until we have at least n terms cached. */
static void generate_up_to(idx_t n)
{
    while (cache_len < n) {
        /* Skip candidates that cannot have a square ending in 1.
           For a square to end in 1, the base must end in 1 or 9. */
        while (true) {
            unsigned last = (unsigned)(next_candidate % 10);
            if (last == 1 || last == 9) break;
            ++next_candidate;
        }

        if (passes(next_candidate)) {
            ensure_cap(cache_len + 1);
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 for safety */
    generate_up_to(n);
    return cache[n - 1];
}
