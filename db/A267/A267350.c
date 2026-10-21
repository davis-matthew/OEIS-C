/*
 * A267350  Binary representation of the n-th iteration of the "Rule 123"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10101 * a(n-2) - 1010100 * a(n-4) + 1000000 * a(n-6) for
 *            n > 6 Initial values: a(0) = 1, a(1) = 101, a(2) = 1110, a(3) =
 *            1110111, a(4) = 111000, a(5) = 11111011111, a(6) = 11100000
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* Base cases (handled for n <= 6) */
    const seq_t base[7] = {
        (seq_t)1,
        (seq_t)101,
        (seq_t)1110,
        (seq_t)1110111,
        (seq_t)111000,
        (seq_t)11111011111,
        (seq_t)11100000
    };

    if (n < 7) {
        return base[n];
    }

    /* Allocate array to hold computed values */
#ifdef __cplusplus
    std::vector<seq_t> arr((size_t)n + 1);
    for (size_t i = 0; i < 7; ++i) {
        arr[i] = base[i];
    }
#else
    seq_t *arr = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));
    if (!arr) {
        /* Allocation failed; return 0 as safe fallback */
        return (seq_t)0;
    }
    memcpy(arr, base, 7 * sizeof(seq_t));
#endif

    /* Compute sequence using the linear recurrence */
    for (idx_t i = 7; i <= n; ++i) {
#ifdef __cplusplus
        arr[i] = (seq_t)10101 * arr[i - 2]
               - (seq_t)1010100 * arr[i - 4]
               + (seq_t)1000000 * arr[i - 6];
#else
        arr[i] = (seq_t)10101 * arr[i - 2]
               - (seq_t)1010100 * arr[i - 4]
               + (seq_t)1000000 * arr[i - 6];
#endif
    }

    seq_t result = arr[n];

#ifndef __cplusplus
    free(arr);
#endif
    return result;
}
