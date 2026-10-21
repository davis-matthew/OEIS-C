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
 * Helpers:   next_unused_b, seq_array_contains
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
#include "../utils/next_unused_b.h"
#include "../utils/seq_array_contains.h"

/* Helper: grow a dynamic array */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t newcap = (*cap < 8 ? 8 : *cap);
        while (newcap < need) newcap <<= 1;
        *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
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
        a_arr = (seq_t *)malloc(a_cap * sizeof(seq_t));
        b_arr = (seq_t *)malloc(b_cap * sizeof(seq_t));

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
