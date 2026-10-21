/*
 * A289559  Number of modulo n residues among sums of two fourth powers.
 * https://oeis.org/A289559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = number of distinct residues r mod n such that r ≡ i^4 +
 *            j^4 (mod n) for some integers i and j. Implementation: compute
 *            all fourth‑power residues modulo n, then all pairwise sums of
 *            those residues modulo n, and count the number of distinct sums.
 *            The result fits in seq_t
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* sequence is defined only for n ≥ 1 */
    if (n == 0) return 0;

    /* n is small enough for array allocation – we assume n ≤ SIZE_MAX. */
    size_t sz = (size_t)n;

    /* Boolean array for fourth‑power residues modulo n */
    bool *fourth = (bool *)calloc(sz, sizeof(bool));
    if (!fourth) return 0;              /* memory allocation failure */

    /* Compute all fourth powers modulo n and mark them */
    for (idx_t x = 0; x < n; ++x) {
        seq_t xx   = (seq_t)x;
        seq_t x2   = (xx * xx) % (seq_t)n;   /* x² mod n */
        seq_t x4   = (x2 * x2) % (seq_t)n;   /* x⁴ mod n */
        /* x4 is guaranteed < n; cast to size_t for indexing */
        fourth[(size_t)x4] = true;
    }

    /* Boolean array for sums of two fourth powers modulo n */
    bool *sums = (bool *)calloc(sz, sizeof(bool));
    if (!sums) {
        free(fourth);
        return 0;
    }

    /* Compute all sums (i + j) mod n for i, j with fourth[i] and fourth[j] true */
    for (idx_t i = 0; i < n; ++i) {
        if (!fourth[(size_t)i]) continue;
        for (idx_t j = 0; j < n; ++j) {
            if (!fourth[(size_t)j]) continue;

            /* Compute (i + j) mod n without overflow */
            idx_t sum;
            if (i > n - j) {                /* i + j > n */
                sum = i + j - n;
            } else {                        /* i + j ≤ n */
                sum = i + j;
                if (sum == n) sum = 0;     /* exactly n → 0 */
            }
            sums[(size_t)sum] = true;
        }
    }

    /* Count distinct residues that can be represented */
    seq_t count = (seq_t)0;
    for (idx_t r = 0; r < n; ++r) {
        if (sums[(size_t)r]) count = count + (seq_t)1;
    }

    /* Clean up */
    free(fourth);
    free(sums);

    return count;
}
