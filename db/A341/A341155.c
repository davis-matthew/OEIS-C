/*
 * A341155  Number of partitions of n into 2 distinct nonzero decimal
 *          palindromes.
 * https://oeis.org/A341155
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of unordered partitions of n into two distinct
 *            non‑zero decimal palindromes
 * Range:     u8 n <= 8845, u16 n <= 8845, u32 n <= 8845, u64 n <= 8845,
 *            u128 n <= 8845, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>   /* for memset */

/* idx_t and seq_t are defined by the build environment. */

/* Test whether an integer is a decimal palindrome. */
static bool is_decimal_palindrome(idx_t x)
{
    idx_t original = x;
    idx_t rev = 0;
    while (x > 0) {
        rev = rev * 10 + x % 10;
        x /= 10;
    }
    return rev == original;
}

/* Cached data for palindromes up to the largest requested n. */
static idx_t          max_n_seen = 0;        /* largest n for which data is ready    */
static idx_t         *pals      = NULL;      /* array of palindromes                */
static size_t        pals_len   = 0;          /* number of palindromes in pals       */
static size_t        pals_cap   = 0;          /* capacity of pals array              */
static uint8_t      *ispal_arr = NULL;       /* 0/1 bitmap of palindrome membership*/
static size_t        ispal_cap = 0;          /* capacity of bitmap                  */

/* Ensure that palindromes up to `limit` are pre‑computed and cached. */
static void init_up_to(idx_t limit)
{
    if (limit <= max_n_seen)
        return;                                 /* already up to date */

    /* Expand bitmap for palindrome membership. */
    if (ispal_cap < (size_t)(limit + 1)) {
        size_t new_cap = ispal_cap ? (ispal_cap << 1) : 64;
        while (new_cap < (size_t)(limit + 1))
            new_cap <<= 1;
        uint8_t *new_arr = (uint8_t*)realloc(ispal_arr, new_cap);
        if (!new_arr)
            abort();                            /* out of memory */
        if (new_cap > ispal_cap)
            memset(new_arr + ispal_cap, 0, new_cap - ispal_cap);
        ispal_arr = new_arr;
        ispal_cap = new_cap;
    }

    /* Allocate initial space for palindrome list if needed. */
    if (pals_cap == 0) {
        pals_cap = 128;
        pals = (idx_t*)realloc(pals, pals_cap * sizeof(idx_t));
        if (!pals)
            abort();
    }

    /* Scan numbers from max_n_seen+1 up to limit,
       adding palindromes to the cache. */
    for (idx_t x = max_n_seen + 1; x <= limit; ++x) {
        if (is_decimal_palindrome(x)) {
            ispal_arr[x] = 1;
            if (pals_len == pals_cap) {
                pals_cap <<= 1;
                idx_t *tmp = (idx_t*)realloc(pals, pals_cap * sizeof(idx_t));
                if (!tmp)
                    abort();
                pals = tmp;
            }
            pals[pals_len++] = x;
        }
    }

    max_n_seen = limit;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 3)            /* sequence is undefined below 3; return 0 */
        return 0;

    /* Prepare cache up to n. */
    init_up_to(n);

    seq_t count = 0;
    for (size_t i = 0; i < pals_len && pals[i] <= n / 2; ++i) {
        idx_t p = pals[i];
        idx_t q = n - p;
        if (q > p && q <= max_n_seen && ispal_arr[q])
            count += 1;
    }
    return count;
}
