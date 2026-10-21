/*
 * A255379  Number of strings of n decimal digits that contain at least one
 *          string of exactly 9 consecutive "0" digits.
 * https://oeis.org/A255379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 13, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A255379: Number of strings of n decimal digits that contain at least one string of exactly 9 consecutive "0" digits.
   Formula implemented: a(n) = 0 for 0 ≤ n < 9, a(9) = 1,
   a(n) = 9 * (10^(n-10) - a(n-10) + Σ_{i=9}^{n-1} a(i)),  n ≥ 10
*/

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mul_small.h"

static seq_t *a_seq = NULL;              /* a[n] values */
static size_t a_cap = 0, a_sz = 0;       /* array capacity and current size */

static seq_t *pow10_seq = NULL;          /* pow10[n] = 10^n */
static size_t pow10_cap = 0, pow10_sz = 0;

/* cumulative sum Σ_{i=9}^k a[i] for the largest computed k */
static seq_t cumSum = 0;

/* Grow an array to accommodate at least 'need' elements */
static void grow_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = *cap ? *cap * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Extend computation up to at least index 'n' */
static void ensure_up_to(idx_t n)
{
    /* Initialize arrays on first use */
    if (a_seq == NULL) {
        grow_array(&a_seq, &a_cap, n + 1 > 16 ? n + 1 : 16);
        grow_array(&pow10_seq, &pow10_cap, n + 1 > 16 ? n + 1 : 16);
        a_sz = 0;
        pow10_sz = 0;
        cumSum = 0;
    }

    /* Compute missing entries sequentially */
    for (idx_t i = a_sz; i <= n; ++i) {
        size_t idx = (size_t)i;

        /* Ensure arrays have space */
        if (idx >= a_cap) grow_array(&a_seq, &a_cap, idx + 1);
        if (idx >= pow10_cap) grow_array(&pow10_seq, &pow10_cap, idx + 1);

        /* Compute 10^i */
        if (i == 0) {
            pow10_seq[0] = (seq_t)1;
        } else {
            pow10_seq[idx] = mul_small(pow10_seq[idx - 1], (unsigned int)10);
        }

        /* Compute a[i] */
        if (i < 9) {
            a_seq[idx] = (seq_t)0;
        } else if (i == 9) {
            a_seq[idx] = (seq_t)1;
        } else {
            seq_t power = pow10_seq[idx - 10];           /* 10^(i-10) */
            seq_t temp  = power - a_seq[idx - 10];        /* 10^(i-10) - a[i-10] */
            temp += cumSum;                              /* + Σ_{k=9}^{i-1} a[k] */
            a_seq[idx] = mul_small(temp, (unsigned int)9);
        }

        /* Update cumulative sum for i ≥ 9 */
        if (i >= 9) {
            cumSum += a_seq[idx];
        }
    }
    a_sz = (size_t)n + 1;
    pow10_sz = a_sz;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return a_seq[(size_t)n];
}
