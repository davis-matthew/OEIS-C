/*
 * A280649  Numbers k such that k^3 has an odd number of digits and the middle
 *          digit is 9.
 * https://oeis.org/A280649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 3315, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cast_uint64_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that k^3 has an odd number of digits and the middle
 * digit is 9.  The function builds the sequence lazily and caches the
 * results so that calls can arrive in any order. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/cast_uint64_to_seq.h"

seq_t a(idx_t n)
{
    /* Static cache of discovered terms */
    static seq_t *list          = NULL;   /* array of seq_t values */
    static size_t list_sz       = 0;      /* number of stored terms   */
    static size_t list_cap      = 0;      /* current allocation size */
    static idx_t next_k         = 1;      /* next integer to test     */

    /* Grow the cache until it contains at least n terms */
    while (list_sz < (size_t)n) {
        /* Compute k^3 using unsigned __int128 to avoid overflow for
         * k up to a few million.
         */
        unsigned __int128 kk   = (unsigned __int128)next_k;
        unsigned __int128 cube = kk * kk * kk;

        /* Compute the number of decimal digits of cube. */
        unsigned __int128 tmp = cube;
        unsigned int len = 0;
        while (tmp > 0) {
            len++;
            tmp /= 10;
        }

        /* Check the odd length condition and whether the middle digit
         * is 9.  For a length 'len' the middle digit comes after
         * 10^(len - (len/2) - 1) places from the right.
         */
        if ((len & 1) == 1) {
            unsigned int mid_shift = len - (len >> 1) - 1;
            unsigned __int128 pow10 = 1;
            for (unsigned int i = 0; i < mid_shift; ++i)
                pow10 *= 10U;

            unsigned __int128 mid_digit = (cube / pow10) % 10U;
            if (mid_digit == 9U) {
                /* Append next_k to the cache. */
                if (list_sz == list_cap) {
                    list_cap = list_cap ? list_cap * 2 : 4;
                    seq_t *tmp_arr = (seq_t *)realloc(list, list_cap * sizeof(seq_t));
                    if (!tmp_arr) /* allocation failure – return 0 as a sentinel */
                        return (seq_t)0;
                    list = tmp_arr;
                }
                list[list_sz++] = cast_uint64_to_seq((uint64_t)next_k);
            }
        }

        next_k++;     /* test the next integer */
    }

    return list[(size_t)n - 1];
}
