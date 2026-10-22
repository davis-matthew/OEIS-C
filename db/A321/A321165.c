/*
 * A321165  Sum of distinct products i*j with 1 <= i, j <= n.
 * https://oeis.org/A321165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 27, u32 n <= 473, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sum of all distinct products i*j with 1 <= i, j <= n.
 * Computation: iterate over all unordered pairs (i, j) with i <= j,
 * store each product in a bitset, and add each new product once.
 * Complexity: O(n^2/2) time, O(n^2) bits memory (≈12.5 MB for n = 10 000).
 */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Number of distinct products cannot exceed n*n. */
    size_t total   = (size_t)n * (size_t)n;          /* bits needed      */
    size_t words   = (total >> 6) + 1;               /* 64‑bit words     */

    /* Allocate the bitset; cast for C++ compatibility. */
    uint64_t *bits = (uint64_t *)calloc(words, sizeof(uint64_t));
    if (!bits)
        return 0;   /* allocation failure */

    seq_t sum = 0;

    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; ++j) {          /* unordered pairs   */
            size_t prod = (size_t)i * (size_t)j;   /* product ≤ n*n     */
            size_t idx  = prod - 1;                /* zero‑based bit    */

            size_t word = idx >> 6;                /* word number       */
            uint64_t mask = 1ULL << (idx & 63);    /* bit mask          */

            if (!(bits[word] & mask)) {            /* new product */
                bits[word] |= mask;
                sum += (seq_t)prod;
            }
        }
    }

    free(bits);
    return sum;
}
