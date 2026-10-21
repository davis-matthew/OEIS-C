/*
 * A247809  Numbers in decimal representation with distinct digits, such that
 *          in Swedish their digits are in alphabetic order.
 * https://oeis.org/A247809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 591, u32 n <= 992, u64 n <= 992,
 *            u128 n <= 992, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A247809 by enumerating all non‑empty subsets of the digits
   0–9, ordering each subset's digits according to Swedish alphabetical order,
   rejecting subsets whose representation would start with a leading zero
   (except the single digit 0), and returning the numbers sorted numerically. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build environment. */
static int cmp_uint64(const void *a, const void *b)
{
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Cache only built once, regardless of call order. */
    static uint64_t values[1000];    /* temporary 64‑bit storage of numbers */
    static seq_t cache[1000];        /* final cache in required seq_t type */
    static size_t cache_len = 0;

    if (cache_len == 0) {
        /* Swedish alphabetical order of the decimal digits. */
        static const int rank_of_digit[10] = {4,0,8,7,2,1,5,6,9,3};
        static const int rank_to_digit[10] = {1,5,4,9,0,6,7,3,2,8};

        size_t len = 0;
        for (uint16_t mask = 1; mask < (1u << 10); ++mask) {
            /* Reject subsets that would lead with zero (except {0}). */
            if (mask & 1u) {
                int min_rank = 10;
                for (int d = 0; d < 10; ++d) {
                    if (mask & (1u << d))
                        if (rank_of_digit[d] < min_rank)
                            min_rank = rank_of_digit[d];
                }
                if (min_rank == rank_of_digit[0] && mask != 1u)
                    continue;                 /* leading zero, skip */
            }

            /* Build the decimal number for this subset. */
            uint64_t val = 0;
            for (int r = 0; r < 10; ++r) {
                int d = rank_to_digit[r];
                if (mask & (1u << d))
                    val = val * 10 + d;
            }
            values[len++] = val;
        }

        /* Sort the numbers numerically. */
        qsort(values, len, sizeof(uint64_t), cmp_uint64);

        /* Fill the final cache converting to the required sequence type. */
        cache_len = len;
        for (size_t i = 0; i < len; ++i)
            cache[i] = (seq_t)values[i];
    }

    if (n == 0 || n > cache_len)
        return 0;           /* out of known range – not expected for valid n */

    return cache[n - 1];
}
