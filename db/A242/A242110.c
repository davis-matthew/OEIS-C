/*
 * A242110  Number of partitions of n whose different summands alternate in
 *          parity.
 * https://oeis.org/A242110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n whose distinct summands
 *            alternate in parity
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 163, u64 n <= 532,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <utility>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Allocate arrays of size n+1 for both parities. */
    seq_t *prevEven = NULL, *prevOdd = NULL;
    seq_t *currEven = NULL, *currOdd = NULL;
    seq_t *add     = NULL;

#if defined(__cplusplus)
    prevEven = new seq_t[n + 1]();
    prevOdd  = new seq_t[n + 1]();
    currEven = new seq_t[n + 1]();
    currOdd  = new seq_t[n + 1]();
    add      = new seq_t[n + 1]();
#else
    prevEven = (seq_t*)calloc(n + 1, sizeof(seq_t));
    prevOdd  = (seq_t*)calloc(n + 1, sizeof(seq_t));
    currEven = (seq_t*)calloc(n + 1, sizeof(seq_t));
    currOdd  = (seq_t*)calloc(n + 1, sizeof(seq_t));
    add      = (seq_t*)calloc(n + 1, sizeof(seq_t));
#endif

    /* Base: empty partition counted once (parity irrelevant). */
    prevEven[0] = (seq_t)1;
    prevOdd[0]  = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        prevEven[i] = prevOdd[i] = (seq_t)0;
    }

    /* Main DP: iterate over possible largest part k. */
    for (idx_t k = 1; k <= n; ++k) {
        /* Zero add[0 .. k-1] */
        for (idx_t i = 0; i < k; ++i) add[i] = (seq_t)0;

        /* Parity of current largest part. 0 = even, 1 = odd. */
        idx_t parity = k & 1;
        /* Source array corresponds to the opposite parity. */
        seq_t *src = (parity == 0) ? prevOdd : prevEven;

        /* Compute cumulative sums for the add array. */
        for (idx_t m = k; m <= n; ++m) {
            add[m] = add[m - k] + src[m - k];
        }

        /* Copy previous values to current arrays. */
        for (idx_t i = 0; i <= n; ++i) {
            currEven[i] = prevEven[i];
            currOdd[i]  = prevOdd[i];
        }

        /* Add contributions where parity matches current part. */
        if (parity == 0) {
            for (idx_t i = 0; i <= n; ++i)
                currEven[i] += add[i];
        } else {
            for (idx_t i = 0; i <= n; ++i)
                currOdd[i] += add[i];
        }

        /* Swap current and previous for next iteration. */
#if defined(__cplusplus)
        std::swap(prevEven, currEven);
        std::swap(prevOdd, currOdd);
#else
        seq_t *tmp = prevEven; prevEven = currEven; currEven = tmp;
        tmp = prevOdd;  prevOdd = currOdd;  currOdd = tmp;
#endif
    }

    /* Result: sum over both possible parities of the largest distinct part. */
    seq_t result = prevEven[n] + prevOdd[n];

    /* Free allocated memory. */
#if defined(__cplusplus)
    delete[] prevEven;
    delete[] prevOdd;
    delete[] currEven;
    delete[] currOdd;
    delete[] add;
#else
    free(prevEven);
    free(prevOdd);
    free(currEven);
    free(currOdd);
    free(add);
#endif

    return result;
}
