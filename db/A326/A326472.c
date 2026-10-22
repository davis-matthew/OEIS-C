/*
 * A326472  Sum of the second largest parts of the partitions of n into 9
 *          parts.
 * https://oeis.org/A326472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of second largest parts of the partitions of n into 9
 *            parts. Computed via partition generating function: a(n) = p_9(n)
 *            + Σ_{j=2..9} T_j(n-9), where p_9(n) = number of partitions of
 *            n-9 into at most 9 parts, and T_j(m) = coefficient of z^m in
 *            z^j/(1 - z^j)^2 ∏_{i≠j} 1/(1 - z^i)
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Static table of partition counts for integer m into at most 9 parts */
static seq_t *coeff = NULL;
static size_t coeff_cap = 0;

/* Ensure coeff array contains values for all m <= up_to */
static void ensure_coeff(idx_t up_to)
{
    if (coeff && coeff_cap > (size_t)up_to) return;

    size_t new_cap = (size_t)up_to + 1;
    seq_t *new_coeff = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_coeff) {
        /* Allocation failure – abort the program (no other choice). */
        exit(1);
    }

    /* Zero initialize */
    memset(new_coeff, 0, new_cap * sizeof(seq_t));
    new_coeff[0] = (seq_t)1;            /* one way to partition 0 */

    /* Partition DP: count partitions into parts of size <= 9 */
    for (int i = 1; i <= 9; ++i) {
        for (size_t w = i; w < new_cap; ++w) {
            new_coeff[w] += new_coeff[w - i];
        }
    }

    /* Free old table if present */
    if (coeff) free(coeff);

    coeff = new_coeff;
    coeff_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 9) return (seq_t)0;  /* no partitions into 9 parts */

    idx_t M = n - 9;              /* m = n - 9 */
    ensure_coeff(M);

    /* p_9(n) = coeff[M] */
    seq_t result = coeff[(size_t)M];

    /* Add contributions from T_j for j = 2 .. 9 */
    for (int j = 2; j <= 9; ++j) {
        /* Sum coeff[M - t*j] for all t >= 1 with t*j <= M */
        for (idx_t t = j; t <= M; t += j) {
            result += coeff[(size_t)(M - t)];
        }
    }

    return result;
}
