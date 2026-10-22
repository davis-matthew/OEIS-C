/*
 * A330861  Number of ways to represent n as a sum of 2 triangular numbers and
 *          a perfect square.
 * https://oeis.org/A330861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 442, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts representations n = T(i)+T(j)+k² with i ≤ j and i,j,k ≥ 0.
   T(m) = m*(m+1)/2 is the m‑th triangular number.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;

    /* Allocate an array of size n+1 to hold the number of unordered
       triangular pairs that sum to each value 0..n.  Use calloc so that
       every element starts at zero, which works for both plain integer
       types and arbitrary‑precision classes. */
    size_t len = (size_t)(n + 1);
    seq_t *pair_counts = (seq_t *)calloc(len, sizeof(seq_t));
    if (!pair_counts) return (seq_t)0;  /* out of memory – return 0 */

    /* Count unordered pairs (i, j) with i ≤ j and T(i)+T(j) = s (0 ≤ s ≤ n). */
    for (idx_t a = 0;; ++a) {
        seq_t Ta = ((seq_t)a) * ((seq_t)(a + 1)) / (seq_t)2;
        if (Ta > N) break;
        for (idx_t b = a;; ++b) {
            seq_t Tb = ((seq_t)b) * ((seq_t)(b + 1)) / (seq_t)2;
            seq_t sum = Ta + Tb;
            if (sum > N) break;
            pair_counts[(size_t)sum] += 1;
        }
    }

    /* Sum the contributions of all square terms k². */
    seq_t result = (seq_t)0;
    for (idx_t k = 0;; ++k) {
        seq_t k2 = ((seq_t)k) * ((seq_t)k);
        if (k2 > N) break;
        seq_t rem = N - k2;               /* remainder after subtracting k² */
        result += pair_counts[(size_t)rem];
    }

    free(pair_counts);
    return result;
}
