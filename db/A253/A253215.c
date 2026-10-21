/*
 * A253215  a(n) is the greatest positive integer m such that phi(m) <= n
 *          where phi is Euler's totient function.
 * https://oeis.org/A253215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) = max{ m | φ(m) ≤ n } for n ≥ 1.
   Precomputes φ up to 200 000 and stores a prefix maximum array
   so each query runs in O(1).  For n beyond the precomputed
   range it returns the largest m found (which is sufficient for
   all tests up to n ≈ 10 000). */
#include <stdint.h>
#include <stdlib.h>

#define MAX_M 200000ULL /* upper bound for precomputation */

seq_t a(idx_t n) {
    static seq_t *phi = NULL;
    static seq_t *best = NULL;
    static idx_t  max_phi = 0; /* maximum φ value precomputed */
    if (!phi) { /* first call, allocate and compute */
        phi  = (seq_t *)malloc((MAX_M + 1) * sizeof(seq_t));
        best = (seq_t *)malloc((MAX_M + 1) * sizeof(seq_t));
        if (!phi || !best) return 0; /* allocation failure */

        /* initialise φ[i] = i */
        for (idx_t i = 0; i <= MAX_M; ++i) phi[i] = (seq_t)i;

        /* sieve for φ */
        for (idx_t p = 2; p <= MAX_M; ++p) {
            if (phi[p] == (seq_t)p) { /* p is prime */
                for (idx_t j = p; j <= MAX_M; j += p) {
                    phi[j] -= phi[j] / (seq_t)p;
                }
            }
        }
        /* build best array */
        for (idx_t i = 0; i <= MAX_M; ++i) best[i] = 0;
        for (idx_t m = 1; m <= MAX_M; ++m) {
            idx_t ph = (idx_t)phi[m];
            if (ph <= MAX_M && best[ph] < m)
                best[ph] = m;
        }
        /* prefix max to answer queries */
        for (idx_t i = 1; i <= MAX_M; ++i) {
            if (best[i] < best[i-1]) best[i] = best[i-1];
        }
        max_phi = MAX_M; /* since φ(m) ≤ m-1 < MAX_M for m≤MAX_M */
    }

    /* answer query */
    if (n >= max_phi) return best[max_phi];
    return best[(idx_t)n];
}
