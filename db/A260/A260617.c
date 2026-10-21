/*
 * A260617  Three-dimensional array read by shells: S(i, j, k) = i + j + k; i
 *          >= 0, j >= 0, k >= 0.
 * https://oeis.org/A260617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A260617: enumerate triples (i,j,k) ≥ 0 in
   shells of increasing maximum value, lexicographically ordered,
   and return the sum i+j+k of the n‑th triple. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset does not matter, but guard */

    /* Find smallest m such that (m+1)^3 ≥ n.
       m is the shell number (max(i,j,k)). */
    idx_t m = 0;
    while (((__uint128_t)(m + 1) * (m + 1) * (m + 1)) < n)
        ++m;

    /* Total triples before this shell = m^3.  Offset inside the shell. */
    idx_t prev   = m * m * m;      /* m^3 fits in idx_t for all n of interest */
    idx_t offset = n - prev - 1;   /* 0‑based index inside shell */

    /* Enumerate triples in lexicographic order, skipping interior m‑1 cube. */
    for (idx_t i = 0; i <= m; ++i) {
        for (idx_t j = 0; j <= m; ++j) {
            for (idx_t k = 0; k <= m; ++k) {
                /* Skip triples where all coordinates < m (interior cube). */
                if (i < m && j < m && k < m)
                    continue;
                if (offset == 0)
                    return (seq_t)(i + j + k);
                --offset;
            }
        }
    }
    /* Should never reach here for valid input. */
    return 0;
}
