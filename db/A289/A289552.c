/*
 * A289552  Zeroless pandigital numbers (each digit 1-9 used exactly once)
 *          where the first 3 digits plus the next 3 digits equals the last 3
 *          digits.
 * https://oeis.org/A289552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is the n-th smallest 9‑digit number formed from the digits
 *            1–9 exactly once, such that the first three digits plus the next
 *            three digits equal the last three digits. The sequence is
 *            indexed from 1
 * Range:     u8 none, u16 none, u32 n <= 336, u64 n <= 336, u128 n <= 336,
 *            big any n
 * Helpers:   reverse_int_range, swap_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: The implementation enumerates all 9! = 362 880
   permutations once, keeps the solutions in an array,
   sorts them, and returns the requested element.
   It is valid for any seq_t/idx_t types that satisfy
   the arithmetic and comparison requirements. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/reverse_int_range.h"
#include "../utils/swap_int.h"

/* --------------------------------------------------------------------- */
/* Utility for soon. ----------------------------------------------------- */

static bool next_permutation(int *a, int n)
{
    int i = n - 2;
    while (i >= 0 && a[i] > a[i + 1]) --i;
    if (i < 0) return false;
    int j = n - 1;
    while (a[j] < a[i]) --j;
    swap_int(&a[i], &a[j]);
    reverse_int_range(a + i + 1, a + n);
    return true;
}

static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* --------------------------------------------------------------------- */
/* Cached array of all sequence values. -------------------------------- */
static seq_t *cache_seq = NULL;
static idx_t cache_count = 0;

/* Build the sequence on first request. -------------------------------- */
static void build_sequence(void)
{
    if (cache_seq != NULL) return;          /* already built */

    const int N = 9;
    int digits[N];
    for (int i = 0; i < N; ++i) digits[i] = i + 1;  /* 1..9 sorted */

    size_t cap = 256;
    size_t len = 0;
    seq_t *arr = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!arr) return;    /* allocation failed */

    do
    {
        int f = digits[0] * 100 + digits[1] * 10 + digits[2];
        int s = digits[3] * 100 + digits[4] * 10 + digits[5];
        int l = digits[6] * 100 + digits[7] * 10 + digits[8];

        if (f + s == l)
        {
            seq_t num = (seq_t)f * (seq_t)1000000
                      + (seq_t)s * (seq_t)1000
                      + (seq_t)l;

            if (len == cap)
            {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(arr, cap * sizeof(seq_t));
                if (!tmp)        /* realloc failed */
                {
                    free(arr);
                    arr = NULL;
                    len = 0;
                    break;
                }
                arr = tmp;
            }
            arr[len++] = num;
        }
    } while (next_permutation(digits, N));

    /* If we never filled the array, keep it empty. */
    if (arr == NULL || len == 0)
    {
        free(arr);
        return;
    }

    /* Sort numerically. */
    qsort(arr, len, sizeof(seq_t), cmp_seq);

    cache_seq   = arr;
    cache_count = (idx_t)len;
}

/* --------------------------------------------------------------------- */
/* Public sequence function. -------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    build_sequence();
    if (n > cache_count) return (seq_t)0; /* out of bounds */
    return cache_seq[n - 1];
}
