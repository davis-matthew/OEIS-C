/*
 * A260308  Expansion of psi(x) * phi(x^3) in powers of x where phi(), psi()
 *          are Ramanujan theta functions.
 * https://oeis.org/A260308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; For n≥1, a(n) = (1/n) * Σ_{k=1..n} ( Σ_{d|k} d*b(d) ) *
 *            a(n‑k), where b(d) is periodic with period 12: b = [ 1, −1, 3,
 *            −1, 1, −4, 1, −1, 3, −1, 1, −2 ]. This implements the Euler
 *            transform corresponding to the generating function q^(−1/8)
 *            η(q²)² η(q⁶)⁵ / (η(q) η(q³)² η(q¹²)²)
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *A = NULL;          /* a(0..cap)                       */
    static int64_t *C = NULL;        /* C[k] = Σ_{d|k} d*b(d) (signed)  */
    static idx_t cap = 0;            /* current allocated size           */

    /* Compute up to n if needed (initialise for n==0 as well). */
    if (cap == 0 || n > cap) {
        idx_t newcap = n;              /* allocate up to index n          */

        /* Allocate memory for A (sequence values). */
        seq_t *newA = (seq_t *)realloc(A, (newcap + 1) * sizeof(seq_t));
        if (!newA) abort();
        A = newA;

        /* Allocate memory for C (divisor sums). */
        int64_t *newC = (int64_t *)realloc(C, (newcap + 1) * sizeof(int64_t));
        if (!newC) abort();
        C = newC;

        /* initialise C[0..newcap] to 0. */
        for (idx_t i = 0; i <= newcap; ++i) C[i] = 0;

        /* Periodic sequence b(d). */
        static const int8_t btab[12] =
            { 1, -1,  3, -1, 1, -4, 1, -1, 3, -1, 1, -2 };

        /* Accumulate contributions of each divisor d to all multiples k. */
        for (idx_t d = 1; d <= newcap; ++d) {
            int8_t b = btab[(d - 1) % 12];          /* b(d) */
            int64_t contrib = (int64_t)d * (int64_t)b; /* d*b(d) */
            for (idx_t k = d; k <= newcap; k += d)
                C[k] += contrib;
        }

        /* Compute the sequence values a(0..newcap) via the recurrence. */
        A[0] = 1;
        for (idx_t m = 1; m <= newcap; ++m) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= m; ++k) {
                seq_t prod = A[m - k];
                prod *= (seq_t)C[k];
                sum += prod;
            }
            A[m] = sum / (seq_t)m;   /* division is exact */
        }

        cap = newcap;
    }
    return A[n];
}
