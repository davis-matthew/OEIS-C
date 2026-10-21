/*
 * A296573  Number of nX3 0..1 arrays with each 1 horizontally, vertically or
 *          antidiagonally adjacent to 0, 1 or 3 neighboring 1s.
 * https://oeis.org/A296573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) +9*a(n-2) +11*a(n-3) -2*a(n-4) -a(n-5)
 *            +10*a(n-6) +2*a(n-7) -29*a(n-8) -16*a(n-9) +27*a(n-10)
 *            +7*a(n-11) -12*a(n-12) -a(n-13) +2*a(n-14)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the above linear recurrence, which holds for all n≥1.
   It allocates a temporary array of length n+1 for each call, initialises
   the first 14 terms, and then iteratively applies the recurrence
   using only integer arithmetic (addition and subtraction). The array
   is freed before returning.  */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                        /* value not defined for n < 1 */

    /* Allocate an array big enough to hold entries up to n (index 0 unused) */
    seq_t *arr = seq_alloc_new_array(n + 1);

    /* First 14 terms (index 1 … 14) */
    static const seq_t base[15] = {0,
        7, 31, 135, 616, 2782, 12544,
        56646, 255699, 1154239, 5210459,
        23520746, 106176061, 479294501,
        2163605794};
    idx_t limit = n < 14 ? n : 14;
    for (idx_t i = 1; i <= limit; ++i)
        arr[i] = base[i];

    /* Coefficients for the recurrence */
    static const int pos_coeffs[8] = { 2, 9, 11, 10,  2, 27,  7,  2 };
    static const int pos_offsets[8] = { 1, 2,  3,  6,  7, 10, 11, 14 };

    static const int neg_coeffs[6] = { 2, 1, 29, 16, 12, 1 };
    static const int neg_offsets[6] = { 4, 5,  8,  9, 12, 13 };

    /* Compute terms from 15 up to n using the recurrence */
    for (idx_t i = 15; i <= n; ++i)
    {
        seq_t sum_pos = 0;
        for (int k = 0; k < 8; ++k)
            sum_pos += arr[i - pos_offsets[k]] * pos_coeffs[k];

        seq_t sum_neg = 0;
        for (int k = 0; k < 6; ++k)
            sum_neg += arr[i - neg_offsets[k]] * neg_coeffs[k];

        arr[i] = sum_pos - sum_neg;         /* result is non‑negative */
    }

    seq_t result = arr[n];
    free_seq_array(arr);                     /* avoid a memory leak */
    return result;
}
