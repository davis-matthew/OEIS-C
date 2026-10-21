/*
 * A279761  Expansion of Product_{k>=1} 1/(1 - x^k)^(k*(2*k^2+1)/3).
 * https://oeis.org/A279761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} 1/(1 - x^k)^(k*(2*k^2+1)/3)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e., a(n) is the Euler transform of the octahedral numbers A005900 */

/* The function computes a(n) using the standard recurrence for Euler transforms: */
/*   a(0) = 1, and for n ≥ 1 */
/*   a(n) = (1/n) * Σ_{k=1}^{n} ( Σ_{d|k} d * b(d) ) * a(n‑k) */
/*   where b(d) = d*(2*d*d+1)/3, the d‑th octahedral number. */

/* Warning: for large n the values grow quickly; when seq_t is a fixed‑width type
   they may overflow, but the sequence is required to be exact only when it
   fits in seq_t.  For arbitrary‑precision builds the correctness is guaranteed. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *A = NULL;      /* values for indices 0 … max-1 */
    static size_t max = 0;       /* current cache size */

    size_t need = (size_t)n + 1; /* we need indices 0 … n */

    if (need > max) {
        /* Reallocate cache for required size */
        seq_t *A_new = (seq_t *)calloc(need, sizeof(seq_t));
        if (!A_new) return 0;   /* out of memory – caller should handle */

        /* Temporary array t[k] = Σ_{d|k} d * b(d) for 1≤k<need */
        seq_t *t = (seq_t *)calloc(need, sizeof(seq_t));
        if (!t) {
            free(A_new);
            return 0;
        }

        /* Compute the divisor sums */
        for (idx_t d = 1; d < need; ++d) {
            seq_t bd = (seq_t)d * (((seq_t)2 * d * d) + 1) / 3;  /* b(d) */
            seq_t term = (seq_t)d * bd;                         /* d * b(d) */
            for (idx_t k = d; k < need; k += d)
                t[k] += term;
        }

        /* Base case: a(0) = 1 */
        A_new[0] = 1;

        /* Recurrence */
        for (idx_t N = 1; N < need; ++N) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= N; ++k)
                sum += t[k] * A_new[N - k];
            A_new[N] = sum / (seq_t)N;   /* exact division */
        }

        /* Replace old cache */
        free(A);
        free(t);
        A = A_new;
        max = need;
    }

    return A[(size_t)n];
}
