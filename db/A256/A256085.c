/*
 * A256085  Non-palindromic balanced numbers in base 5.
 * https://oeis.org/A256085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth non‑palindromic balanced number in base 5 (computed
 *            by brute‑force search)
 * Range:     u8 n <= 4, u16 n <= 2946, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* cache of already found terms */
static seq_t *cache = NULL;      /* storage for values */
static size_t cache_size = 0;    /* how many terms we have */
static size_t cache_cap   = 0;   /* allocated capacity */
static idx_t last_checked = 0;   /* largest integer examined */

/* grow the cache so that it can hold “needed” elements */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *tmp = alloc_seq_array(new_cap);
    if (!tmp) abort();        /* failure unlikely in the tests */

    /* copy existing data */
    for (size_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
    if (cache) free_seq_array(cache);

    cache = tmp;
    cache_cap = new_cap;
}

/* test whether v (in base‑5) is balanced and not a palindrome
   Balanced: Σ d_i·(2·i – (len‑1)) == 0
   Palindrome check done after balance check. */
static bool balanced_nonpal(idx_t v)
{
    if (v == 0) return false;       /* zero is ignored */

    uint8_t digs[64];
    size_t len = 0;
    idx_t t = v;

    /* convert to base‑5; digits stored LSB first */
    while (t) {
        digs[len++] = (uint8_t)(t % 5);
        t /= 5;
    }

    /* reverse to MSB first for easier weight calculation */
    for (size_t i = 0; i < len / 2; ++i) {
        uint8_t tmp = digs[i];
        digs[i] = digs[len - 1 - i];
        digs[len - 1 - i] = tmp;
    }

    int sum = 0;
    for (size_t i = 0; i < len; ++i)
        sum += (int)digs[i] * (int)(2 * i - (len - 1));

    if (sum != 0) return false;          /* not balanced */

    /* palindrome test */
    for (size_t i = 0; i < len / 2; ++i)
        if (digs[i] != digs[len - 1 - i]) return true; /* balanced & non‑palin */

    return false;                        /* balanced but palindrome */
}

/* --- public interface ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined for offset 1 */

    /* generate terms until we have at least n values cached */
    while (cache_size < (size_t)n) {
        /* find the next balanced non‑palindromic number */
        for (++last_checked; ; ++last_checked) {
            if (balanced_nonpal(last_checked)) {
                ensure_capacity(cache_size + 1);
                cache[cache_size++] = (seq_t)last_checked;
                break;
            }
        }
    }

    return cache[n - 1];
}
