/*
 * A394317  Concatenated permutations of integers in decimal.
 * https://oeis.org/A394317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabf
 * Formula:   a(n) = the n‑th element of the triangle of concatenated
 *            permutations of 0,…,k‑1 in lexicographic order, where k is the
 *            unique integer satisfying Σ_{i=1}^{k-1} i! < n ≤ Σ_{i=1}^{k} i!
 *            . The construction interprets each permutation as a decimal
 *            integer, dropping leading zeros
 * Range:     u8 n <= 13, u16 n <= 273, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* find k such that cumulative factorial sum reaches n */
    idx_t total = 0;        /* cumulative sum of factorials up to previous k */
    idx_t k = 0;            /* current row index (1‑based) */
    idx_t fact = 1;         /* k! as idx_t */

    while (1) {
        k += 1;
        fact *= k;          /* fact = k! */
        if (n <= total + fact)
            break;
        total += fact;
    }

    idx_t pos = n - total - 1; /* zero‑based index within this row */

    /* precompute factorials up to k */
    idx_t fac[21];          /* factorial array, 0! = 1 */
    fac[0] = 1;
    for (idx_t i = 1; i <= k; ++i)
        fac[i] = fac[i - 1] * i;

    /* build the pos‑th lexicographic permutation of digits 0..k-1 */
    uint8_t digits[21];
    for (idx_t i = 0; i < k; ++i)
        digits[i] = (uint8_t)i;

    seq_t result = 0;
    idx_t avail = k;        /* number of digits remaining */

    for (idx_t i = 0; i < k; ++i) {
        idx_t block = fac[avail - 1];   /* (avail-1)! */
        idx_t index = pos / block;
        pos %= block;

        uint8_t d = digits[index];
        result = result * 10 + d;      /* concatenate digit */

        /* remove chosen digit from the pool */
        for (idx_t j = index; j < avail - 1; ++j)
            digits[j] = digits[j + 1];
        avail--;
    }

    return result;
}
