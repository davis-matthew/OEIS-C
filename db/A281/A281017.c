/*
 * A281017  Numbers with a prime number of dashes in their International Morse
 *          numeral representation.
 * https://oeis.org/A281017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   free_seq_arr, get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A281017: Numbers with a prime number of dashes in their
 * International Morse numeral representation.
 *
 * The Morse code for each decimal digit is:
 *   0: "-----"  (5 dashes)
 *   1: ".----"  (4 dashes)
 *   2: "..---"  (3 dashes)
 *   3: "...--"  (2 dashes)
 *   4: "....-"  (1 dash)
 *   5: "....."  (0 dashes)
 *   6: "-...."  (1 dash)
 *   7: "--..."  (2 dashes)
 *   8: "---.."  (3 dashes)
 *   9: "----."  (4 dashes)
 *
 * The function a(idx_t n) returns the nth term (1‑based) of the
 * sequence.  It builds the sequence lazily and caches results.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_digits.h"
#include "../utils/free_seq_arr.h"

/* C++ compatibility: allocate a raw array of seq_t objects. */
static seq_t *alloc_seq_arr(size_t cap)
{
#if defined(__cplusplus)
    return new seq_t[cap];
#else
    return (seq_t *)malloc(cap * sizeof(seq_t));
#endif
}

/* C++ compatibility: free a raw array of seq_t objects. */

/* Cache for the sequence values. */
static seq_t *cache = NULL;      /* pointer to array of seq_t */
static size_t cache_size = 0;    /* how many terms are cached */
static size_t cache_cap = 0;     /* allocated capacity */
static seq_t next_candidate = 0; /* next integer to test */

/* Grow the cache array when more space is needed. */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    seq_t *new_cache = alloc_seq_arr(new_cap);
    /* copy existing elements */
    for (size_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];
    free_seq_arr(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Test if a small integer (≤~200) is prime. */
static bool is_small_prime(int x)
{
    if (x < 2) return false;
    for (int i = 2; i * i <= x; ++i)
        if (x % i == 0) return false;
    return true;
}

/* Ensure that the cache contains at least n terms (n ≥ 1). */
static void ensure_cache(idx_t n)
{
    if ((idx_t)n <= cache_size) return;

    /* number of dashes for each decimal digit */
    static const int dash_tbl[10] = {5, 4, 3, 2, 1, 0, 1, 2, 3, 4};

    while (cache_size < (size_t)n)
    {
        seq_t cand = next_candidate;
        /* Count dashes in the Morse representation of cand. */
        size_t len;
        uint8_t digits[128];          /* enough for any 128‑bit number */
        get_digits(cand, digits, &len);

        int dash_cnt = 0;
        for (size_t i = 0; i < len; ++i)
            dash_cnt += dash_tbl[digits[i] & 0xF];

        if (is_small_prime(dash_cnt))
        {
            if (cache_size == cache_cap) grow_cache();
            cache[cache_size++] = cand;
        }
        /* advance to next integer */
        next_candidate = cand + 1;
    }
}

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* behavior below offset is irrelevant */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
