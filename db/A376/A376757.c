/*
 * A376757  Number of pairs 0 <= x <= y <= n-1 such that x^3 == y^3 (mod n).
 * https://oeis.org/A376757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = # of pairs (x, y) with 0 ≤ x ≤ y ≤ n-1 and x^3 ≡ y^3
 *            (mod n). * For each residue r modulo n, let c_r be the number of
 *            x in [0, n-1] * satisfying x^3 ≡ r (mod n). The contribution of
 *            r to a(n) is * c_r * (c_r + 1) / 2. Thus * a(n) = Σ_{r=0}^{n-1}
 *            c_r * (c_r + 1) / 2
 * Range:     u8 n <= 62, u16 n <= 3590, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, return 0 as a safe default */

    /* Allocate an array to store counts of each residue class. */
    idx_t *cnt = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (cnt == NULL) return 0;     /* allocation failure → 0 as fallback */

    /* Initialise counts to zero. */
    for (idx_t i = 0; i < n; ++i) cnt[i] = 0;

    /* Count how many x produce each residue x^3 mod n. */
    for (idx_t x = 0; x < n; ++x) {
        idx_t r = ((x * x) % n * x) % n;   /* x^3 mod n, calculated in idx_t */
        ++cnt[r];
    }

    /* Sum the contributions of each residue class. */
    seq_t total = 0;
    for (idx_t r = 0; r < n; ++r) {
        idx_t c = cnt[r];
        if (c > 0) {
            total += (seq_t)c * (c + 1) / 2;   /* c*(c+1)/2 fits in seq_t */
        }
    }

    free(cnt);
    return total;
}
