/*
 * A330765  Total number of blocks in all set partitions of strict integer
 *          partitions of n.
 * https://oeis.org/A330765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m≥1} T(m) * p(n - m(m-1)/2, m), * where p(n,k) is the
 *            number of unrestricted partitions of n into exactly k parts, *
 *            and T(m) = Σ_{k=1}^m k * S(m,k) with S(m,k) Stirling numbers of
 *            the second kind. * This implements the definition of sequence
 *            A330765
 * Range:     u8 n <= 13, u16 n <= 31, u32 n <= 83, u64 n <= 233,
 *            u128 n <= 683, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state */
    static bool initialized = false;
    static idx_t max_n = 0;
    static idx_t max_m = 0;

    static seq_t *P = NULL;        /* p(n,m) array with size (max_n+1)*(max_m+1) */
    static seq_t *S = NULL;        /* Stirling numbers S(m,k) size (max_m+1)*(max_m+1) */
    static seq_t *T_arr = NULL;    /* T(m) array size (max_m+1) */
    static idx_t *shift_arr = NULL;/* shift m(m-1)/2 array size (max_m+1) */

    /* Determine necessary sizes for the current call */
    idx_t needed_max_m = 0;
    while ((needed_max_m + 1) * (needed_max_m + 2) / 2 <= n)
        ++needed_max_m;

    if (!initialized || n > max_n || needed_max_m > max_m) {
        /* recompute for the larger bounds */
        max_n = n;
        max_m = needed_max_m;

        /* allocate p(n,m) array */
        size_t colP = (size_t)(max_m + 1);
        size_t rowsP = (size_t)(max_n + 1);
        seq_t *newP = (seq_t *)calloc(rowsP * colP, sizeof(seq_t));
        if (!newP) { abort(); }  /* allocation failure */
        /* DP for unrestricted partitions with exactly k parts */
        newP[0 * colP + 0] = (seq_t)1;
        for (idx_t i = 1; i <= max_n; ++i) {
            for (idx_t m = 1; m <= max_m; ++m) {
                if (m > i) { /* zero already */
                    continue;
                }
                seq_t val = newP[(i - 1) * colP + (m - 1)];
                if (i >= m) {
                    val += newP[(i - m) * colP + m];
                }
                newP[i * colP + m] = val;
            }
        }
        /* free old array */
        free(P);
        P = newP;

        /* allocate Stirling numbers S(m,k) */
        size_t colS = (size_t)(max_m + 1);
        seq_t *newS = (seq_t *)calloc(colS * colS, sizeof(seq_t));
        if (!newS) { abort(); }
        newS[0 * colS + 0] = (seq_t)1;
        for (idx_t i = 1; i <= max_m; ++i) {
            for (idx_t k = 1; k <= i; ++k) {
                /* S(i,k) = S(i-1,k-1) + k * S(i-1,k) */
                seq_t val = newS[(i - 1) * colS + (k - 1)];
                val += (seq_t)k * newS[(i - 1) * colS + k];
                newS[i * colS + k] = val;
            }
        }
        free(S);
        S = newS;

        /* compute shift and T arrays */
        idx_t *newShift = (idx_t *)calloc((size_t)(max_m + 1), sizeof(idx_t));
        seq_t *newT = (seq_t *)calloc((size_t)(max_m + 1), sizeof(seq_t));
        if (!newShift || !newT) { abort(); }
        for (idx_t m = 1; m <= max_m; ++m) {
            newShift[m] = m * (m - 1) / 2;
            seq_t sum = 0;
            for (idx_t k = 1; k <= m; ++k) {
                seq_t term = (seq_t)k * S[m * colS + k];
                sum += term;
            }
            newT[m] = sum;
        }
        free(shift_arr);
        shift_arr = newShift;
        free(T_arr);
        T_arr = newT;

        initialized = true;
    }

    /* compute a(n) using the precomputed data */
    seq_t result = (seq_t)0;
    size_t colP = (size_t)(max_m + 1);
    for (idx_t m = 1; m <= max_m; ++m) {
        idx_t shift = shift_arr[m];
        if (shift > n) continue;
        idx_t remaining = n - shift;
        seq_t part = P[remaining * colP + m];
        if (part) {
            result += T_arr[m] * part;
        }
    }
    return result;
}
