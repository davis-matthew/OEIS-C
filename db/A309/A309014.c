/*
 * A309014  a(n) = Sum_{k=0..n} (-1)^(n-k) * (Stirling2(n,k) mod 2).
 * https://oeis.org/A309014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * (Stirling2(n,k) mod 2)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cached values of the sequence (a(0) .. a(max)) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements stored */

    /* Parity row of the Stirling numbers of the second kind.
       arr[i] holds S(current_n, i) mod 2, with 0 <= i <= current_n. */
    static uint8_t *arr = NULL;
    static size_t arr_cap = 0;     /* allocated capacity */

    /* Initialise tables on first call */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)1;      /* a(0) = 1 */

        arr_cap = 1;
        arr = (uint8_t *)malloc(arr_cap * sizeof(uint8_t));
        if (arr == NULL) abort();
        arr[0] = 1;               /* S(0,0) mod 2 = 1 */
    }

    /* Ensure we have computed up to n */
    while (cache_len <= n) {
        /* Prepare for the next n */
        idx_t cur_n = (idx_t)cache_len;      /* current n to compute */

        /* Resize arr if necessary */
        if ((size_t)(cur_n + 1) > arr_cap) {
            arr_cap = cur_n + 1;
            uint8_t *tmp = (uint8_t *)realloc(arr, arr_cap * sizeof(uint8_t));
            if (tmp == NULL) abort();
            arr = tmp;
        }

        /* Initialise new element to 0 (S(cur_n, cur_n) = 0 when cur_n > 0) */
        arr[cur_n] = 0;

        /* Update the row in-place from high to low indices */
        for (size_t k = cur_n; k >= 1; --k) {
            if ((k & 1) == 0) {
                /* even k: S(n,k) = S(n-1,k-1) */
                arr[k] = arr[k - 1];
            } else {
                /* odd k: S(n,k) = S(n-1,k) XOR S(n-1,k-1) */
                arr[k] = arr[k] ^ arr[k - 1];
            }
        }

        /* S(cur_n,0) is always 0 for n>0 */
        arr[0] = 0;

        /* Compute a(cur_n) = Σ(-1)^(cur_n-k)*arr[k] */
        long long sum = 0;
        for (size_t k = 0; k <= cur_n; ++k) {
            if (arr[k]) {
                if (((cur_n - k) & 1) == 0)
                    ++sum;
                else
                    --sum;
            }
        }

        /* Append to cache */
        seq_t *tmp = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache[cache_len] = (seq_t)sum;
        ++cache_len;
    }

    return cache[n];
}
