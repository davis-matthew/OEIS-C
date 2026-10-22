/*
 * A399602  The first 25 prime numbers (from 2 to 97) arranged row by row into
 *          a 5 X 5 balanced quasi-magic square. Every outer row and column
 *          sums to 215, while the four lines passing through the central
 *          number 2 (central row, central column, and both diagonals) sum
 *          exactly to 200.
 * https://oeis.org/A399602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, tabl, easy
 * Formula:   a(n) = the entry at position n (1‑based) in the 5×5 *
 *            quasi‑magic square formed from the first 25 primes in ascending
 *            * order. For 1 ≤ n ≤ 25 the square’s entries are obtained by the
 *            * fixed permutation listed below; for other n the function
 *            returns 0. * This implements the OEIS definition for A3...
 * Range:     u8 n <= 25, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The five‑by‑five arrangement read row by row. */
    static const seq_t primes[25] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
        31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL,
        71ULL, 73ULL, 79ULL, 83ULL, 89ULL, 97ULL
    };

    /* Mapping from the 1‑based row‑by‑row position to the index in
     * the ascending primes array (1‑based).  The zero‑based indices are
     * obtained by subtracting 1 when accessing 'primes'. */
    static const int perm[25] = {
        4, 15, 24, 10, 14,   /* row 1 */
        5, 16, 11, 17, 18,   /* row 2 */
        25, 22, 1, 8, 2,     /* row 3 */
        7, 6, 21, 20, 13,    /* row 4 */
        23, 9, 3, 12, 19     /* row 5 */
    };

    if (n < 1 || n > 25) return 0;          /* the sequence is defined only for 1–25 */

    size_t idx = (size_t)(perm[(size_t)(n - 1)] - 1);
    return primes[idx];
}
