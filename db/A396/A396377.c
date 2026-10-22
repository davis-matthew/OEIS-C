/*
 * A396377  Smallest k so that at least one of the k+1 integers in {n, n+1,
 *          n+2, ..., n+k} divides the product of the other integers.
 * https://oeis.org/A396377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ 1 such that some number in * {n, n+1, …,
 *            n+k} divides the product of the other k numbers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Brute‑force increasing k until the condition is met.
       Complexity is O(k²) per test, but k stays very small
       for n ≤ 10000 (typically < 10). */
    for (idx_t k = 1; k <= n; ++k) {
        for (idx_t i = 0; i <= k; ++i) {
            idx_t m = n + i;              /* current test divisor   */
            idx_t prod = 1;               /* product of all others  */
            for (idx_t j = 0; j <= k; ++j) {
                if (j == i) continue;
                prod = (prod * ((n + j) % m)) % m;
            }
            if (prod == 0)                /* division holds        */
                return (seq_t)k;
        }
    }
    /* Theoretically unreachable: for every n we must find some k ≤ n. */
    return (seq_t)n;
}
