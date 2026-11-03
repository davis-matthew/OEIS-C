/*
 * A296557  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n) - n, where a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4, b(2) = 5,
 *          and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 935 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Complementary sequences a(n) and b(n) defined by:
 *
 *   a(0) = 1,  a(1) = 2,  a(2) = 6,  a(3) = 11
 *   b(0) = 3,  b(1) = 4,  b(2) = 5,  b(3) = 7
 *
 *   For n >= 4:
 *     b(n) = smallest unused positive integer > b(n-1)
 *     a(n) = a(n-1) + a(n-2) + b(n) - n
 *
 * The two sequences are increasing and complementary (together
 * they contain all positive integers).
 *
 * The function below lazily computes a(n) on any call order,
 * using memoisation.  It works for arbitrary-width unsigned
 * integer types as required by the build system.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: binary search in a sorted array */
static bool seq_array_contains(const seq_t *arr, size_t len, seq_t val)
{
    size_t low = 0, high = len;
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        if (arr[mid] == val) return true;
        if (arr[mid] < val) low = mid + 1;
        else high = mid;
    }
    return false;
}

/* Find the next unused integer for b:
 * last_b: the previous b value,
 * a_arr:  array of already computed a values,
 * a_len:  number of a values,
 * b_arr:  array of already computed b values (including last_b),
 * b_len:  number of b values          */
static seq_t next_unused_b(seq_t last_b,
                           const seq_t *a_arr, size_t a_len,
                           const seq_t *b_arr, size_t b_len)
{
    seq_t cand = last_b + (seq_t)1;
    for (;;) {
        if (seq_array_contains(a_arr, a_len, cand) ||
            seq_array_contains(b_arr, b_len, cand)) {
            ++cand; /* already used */
        } else {
            return cand;
        }
    }
}

/* Helper: grow a dynamic array */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t newcap = (*cap < 8 ? 8 : *cap);
        while (newcap < need) newcap <<= 1;
        *arr = mem_resize(seq_t, *arr, newcap);
        *cap = newcap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *a_arr = NULL;
    static seq_t *b_arr = NULL;
    static size_t a_cap    = 0, b_cap = 0;
    static size_t a_len    = 0, b_len = 0;
    static idx_t  computed_max = 3;   /* last index for which a is known */

    /* initialise arrays on first call */
    if (a_arr == NULL) {
        a_cap = 8;
        b_cap = 8;
        a_arr = mem_new(seq_t, a_cap);
        b_arr = mem_new(seq_t, b_cap);

        /* a(0) .. a(3) */
        a_arr[0] = (seq_t)1;
        a_arr[1] = (seq_t)2;
        a_arr[2] = (seq_t)6;
        a_arr[3] = (seq_t)11;
        a_len   = 4;

        /* b(0) .. b(3) */
        b_arr[0] = (seq_t)3;
        b_arr[1] = (seq_t)4;
        b_arr[2] = (seq_t)5;
        b_arr[3] = (seq_t)7;
        b_len   = 4;
    }

    /* return cached value if already computed */
    if (n <= computed_max)
        return a_arr[(size_t)n];

    /* start generating from the first missing index */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        /* compute b(i) for i >= 4 */
        if (i >= 4) {
            ensure_capacity(&b_arr, &b_cap, (size_t)i + 1);
            seq_t new_b = next_unused_b(b_arr[(size_t)(i - 1)],
                                        a_arr, a_len,
                                        b_arr, b_len);
            b_arr[(size_t)i] = new_b;
            ++b_len;
        }

        /* compute a(i) using recurrence: a(i)=a(i-1)+a(i-2)+b(i)-i */
        ensure_capacity(&a_arr, &a_cap, (size_t)i + 1);
        seq_t ai = a_arr[(size_t)(i - 1)]
                 + a_arr[(size_t)(i - 2)]
                 + b_arr[(size_t)i]
                 - (seq_t)i;
        a_arr[(size_t)i] = ai;
        ++a_len;
        computed_max = i;
    }

    return a_arr[(size_t)n];
}
