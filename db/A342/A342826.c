/*
 * A342826  Numbers k such that d(1)^0 + d(2)^1 + ... + d(p)^(p-1) =
 *          d(1)^(p-1) + d(2)^(p-2) + ... + d(p)^0, where d(i), i=1..p, are
 *          the digits of k.
 * https://oeis.org/A342826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 898, u32 n <= 1137, u64 n <= 1137,
 *            u128 n <= 1137, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A342826: numbers k such that 
   d(1)^0 + d(2)^1 + … + d(p)^(p-1) == d(1)^(p-1) + d(2)^(p-2) + … + d(p)^0
   where d(i) are the decimal digits of k.
   The function searches integers until the n‑th term is found. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* forward declarations of helper functions */
static bool is_special(seq_t k);
static void grow_seq(seq_t **arr, size_t *cap, size_t needed);

/* static storage for the sequence values (memoization) */
static seq_t *cache    = NULL; /* array of found terms          */
static size_t cache_cap = 0;   /* allocated capacity            */
static size_t cache_len = 0;   /* number of terms stored        */
static seq_t next_candidate = 1; /* next integer to test       */

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure the cache contains at least n terms (OEIS offset is 1). */
    while (cache_len < n) {
        if (is_special(next_candidate)) {
            if (cache_len == cache_cap)
                grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}

/* --------------------------------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap == 0) {
        *cap = 4;
        *arr = (seq_t*)calloc(*cap, sizeof(seq_t));
    } else if (needed > *cap) {
        size_t newcap = *cap << 1;
        if (newcap < needed) newcap = needed;
        *arr = (seq_t*)realloc(*arr, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

/* --------------------------------------------------------------------- */
static seq_t pow_seq(seq_t base, size_t exp)
{
    seq_t result = 1;
    while (exp != 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* --------------------------------------------------------------------- */
static bool is_special(seq_t k)
{
    /* determine number of digits */
    seq_t tmp = k;
    size_t dlen = 0;
    if (tmp == 0) dlen = 1;
    else {
        while (tmp != 0) {
            tmp /= 10;
            ++dlen;
        }
    }

    /* allocate array for digits (least significant first) */
    uint8_t *digits = (uint8_t*)malloc(dlen);
    if (!digits) return false; /* unlikely */

    tmp = k;
    for (size_t i = 0; i < dlen; ++i) {
        digits[i] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    seq_t sum_left = 0, sum_right = 0;
    for (size_t i = 0; i < dlen; ++i) {
        seq_t digit = (seq_t)digits[dlen - 1 - i]; /* most→least     */
        size_t exp_left  = i;           /* exponent for d(i+1)^(i)   */
        size_t exp_right = dlen - 1 - i; /* exponent for d(i+1)^(p-i-1) */

        seq_t l = pow_seq(digit, exp_left);
        seq_t r = pow_seq(digit, exp_right);
        sum_left  += l;
        sum_right += r;
    }

    free(digits);
    return sum_left == sum_right;
}
