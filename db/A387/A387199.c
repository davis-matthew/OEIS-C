/*
 * A387199  Numbers which are not themselves palindromes, but a single swap of
 *          two digits creates a palindrome.
 * https://oeis.org/A387199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 3207, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A387199: Numbers which are not themselves palindromes, but a single swap of two digits creates a palindrome.
    The function searches for such numbers in increasing order, caching results so that calls take
    O(1) amortised time after the first call.  The implementation is valid in C11 and C++17. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Maximum number of decimal digits we expect to handle for the first ten‑thousand terms.
   The sequence grows slowly, so 64 digits is ample for n ≤ 10000. */
#define MAX_DIGITS 64

/* Predicate: does x have the property? */
static bool is_single_swap_palindrome(seq_t x)
{
    if (x == (seq_t)0)          /* zero should never be considered */
        return false;

    /* Extract decimal digits into a buffer in least‑significant‑first order. */
    size_t len = 0;
    seq_t t = x;
    uint8_t digits[MAX_DIGITS];
    while (t > (seq_t)0) {
        if (len >= MAX_DIGITS)        /* safety guard – numbers with >64 digits are ignored */
            return false;
        digits[len++] = (uint8_t)(t % (seq_t)10);
        t /= (seq_t)10;
    }

    /* Build a most‑significant‑first array. */
    uint8_t rev[MAX_DIGITS];
    for (size_t i = 0; i < len; ++i)
        rev[i] = digits[len - 1 - i];

    /* Quick test: the number must not itself be a palindrome. */
    bool orig_pal = true;
    for (size_t i = 0; i + 1 < len; ++i)
        if (rev[i] != rev[len - 1 - i]) {
            orig_pal = false;
            break;
        }
    if (orig_pal)
        return false;

    /* Try every unordered pair of positions i < j. */
    for (size_t i = 0; i < len; ++i) {
        for (size_t j = i + 1; j < len; ++j) {
            if (rev[i] == rev[j])          /* swapping equal digits does nothing */
                continue;

            /* Swap the two digits in place. */
            uint8_t tmp = rev[i];
            rev[i] = rev[j];
            rev[j] = tmp;

            /* Leading zero check for the candidate palindrome. */
            if (rev[0] != 0) {
                bool pal = true;
                for (size_t k = 0; k + 1 < len; ++k)
                    if (rev[k] != rev[len - 1 - k]) {
                        pal = false;
                        break;
                    }
                if (pal) {
                    /* Undo the swap before leaving. */
                    uint8_t tmp2 = rev[i];
                    rev[i] = rev[j];
                    rev[j] = tmp2;
                    return true;
                }
            }

            /* Undo the swap for next iteration. */
            uint8_t tmp2 = rev[i];
            rev[i] = rev[j];
            rev[j] = tmp2;
        }
    }
    return false;
}

/* Cached sequence array. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t last_checked = (seq_t)0;

/* Helper to grow the cache array. */
static void cache_grow(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* unused; sequence is 1‑based */

    /* Ensure the cache contains up to the n‑th term. */
    while (cache_len < (size_t)n) {
        /* Move to the next integer candidate. */
        ++last_checked;

        if (is_single_swap_palindrome(last_checked)) {
            if (cache_len == cache_cap)
                cache_grow();
            cache[cache_len++] = last_checked;
        }
    }

    return cache[(size_t)(n - 1)];
}
