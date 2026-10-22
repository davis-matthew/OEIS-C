/*
 * A389536  G.f. A(x) satisfies: A(x) = A(x^2 + 2*x^3) / x.
 * https://oeis.org/A389536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A(x) = A(x^2 + 2*x^3) / x (offset 1) Recurrence used: a(1)=1 For
 *            n ≥ 2: a(n) = Σ_{k=⌈(n+1)/3⌉}^{⌊(n+1)/2⌋} a(k) * binom(k,
 *            n+1‑2k) * 2^{n+1‑2k} The implementation evaluates this
 *            recurrence by accumulating the contributions for each k once,
 *            which runs in O(n²) time (≈8·10⁶ updates for n=10⁴)
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* sequence is defined only for n ≥ 1 */

    /* allocate arrays for the sequence values and 2^r */
    seq_t *A  = alloc_seq_array(n + 1);   /* indices 0..n ; A[0] unused */
    seq_t *pow2 = alloc_seq_array(n + 1);

    /* initialise to zero and set the first term */
    for (idx_t i = 0; i <= n; ++i) A[i] = 0;
    A[1] = 1;

    /* pre‑compute powers of two: pow2[i] = 2^i */
    pow2[0] = 1;
    for (idx_t i = 1; i <= n; ++i) pow2[i] = pow2[i - 1] * ((seq_t)2);

    /* main recurrence: add contributions of each k to all valid j = 2k + r - 1 */
    idx_t kmax = (n + 1) / 2;           /* only k up to this point can reach indices ≤ n */

    for (idx_t k = 1; k <= kmax; ++k) {
        idx_t rmax = k;
        idx_t temp = n - 2 * k + 1;    /* largest r such that 2k + r - 1 ≤ n */
        if (temp < rmax) rmax = temp;

        seq_t bin = 1;                 /* bin = binom(k, r) during the r loop */

        for (idx_t r = 0; r <= rmax; ++r) {
            if (r > 0) {               /* update binom(k, r) from binom(k, r‑1) */
                bin = bin * (k - r + 1);
                bin = bin / r;
            }

            idx_t j = 2 * k + r - 1;    /* target index for a[j] */
            if (j >= 2 && j <= n) {
                seq_t term = A[k] * bin;
                term = term * pow2[r];
                A[j] += term;
            }
        }
    }

    seq_t result = A[n];
    free_seq_array(A);
    free_seq_array(pow2);
    return result;
}
