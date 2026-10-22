/*
 * A371807  Number of nonoverlapping 666 substrings contained in the decimal
 *          expansion of the n-th apocalyptic number.
 * https://oeis.org/A371807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   A371807 – number of nonoverlapping “666” substrings in the
 *            decimal expansion of the n‑th apocalyptic number (positive power
 *            of 2 that contains 666)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_non_overlapping666
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/count_non_overlapping666.h"

static uint8_t *digits = NULL;          /* least significant digit first */
static size_t   digits_len = 0;         /* current number of digits    */
static size_t   digits_cap = 0;         /* allocated capacity (digits) */
static idx_t    cur_exp = 0;            /* current exponent (2^cur_exp) */
static idx_t    apoc_cnt = 0;           /* number of apocalyptic numbers found so far */

static seq_t   *cache = NULL;          /* cache of answers */
static size_t   cache_cap = 0;         /* cache capacity */
static idx_t    cache_sz  = 0;         /* number of stored answers */

/* Ensure the digit array can hold at least 'need' digits.  */
static void ensure_digits_cap(size_t need)
{
    if (need <= digits_cap) return;
    size_t new_cap = (digits_cap==0) ? 128 : digits_cap;
    while (new_cap < need) new_cap <<= 1;
    uint8_t *p = (uint8_t *)realloc(digits, new_cap);
    if (!p) abort(); /* memory failure */
    digits      = p;
    digits_cap  = new_cap;
}

/* Ensure the cache array can hold at least 'need' results.  */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = (cache_cap==0) ? 64 : cache_cap;
    while (new_cap < need) new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p) abort();
    cache     = p;
    cache_cap = new_cap;
}

/* Multiply the current decimal value by 2 in place. */
static void mul_by_2(void)
{
    uint16_t carry = 0;
    for (size_t i = 0; i < digits_len; ++i) {
        uint16_t prod = (uint16_t)digits[i] * 2 + carry;
        digits[i] = (uint8_t)(prod % 10);
        carry     = prod / 10;
    }
    if (carry) {
        ensure_digits_cap(digits_len + 1);
        digits[digits_len++] = (uint8_t)carry;
    }
}

/* Main routine to compute a(n).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1, just for safety */
    /* Grow cache if necessary */
    if (n > cache_sz) {
        ensure_cache_cap(n);
        /* initialise digit array for 2^0 == 1 if not already ready */
        if (!digits) {
            ensure_digits_cap(1);
            digits[0] = 1;     /* 2^0 */
            digits_len = 1;
            cur_exp = 0;
        }
        /* Compute until we have computed the nth apocalyptic number */
        while (apoc_cnt < n) {
            /* advance to next exponent */
            mul_by_2();
            ++cur_exp;

            /* scan for "666" */
            seq_t occ = count_non_overlapping666(digits, digits_len);
            if (occ) {
                ++apoc_cnt;
                cache[apoc_cnt - 1] = occ;
            }
        }
        cache_sz = apoc_cnt;  /* we now have at least n results */
    }
    return cache[n - 1];
}
