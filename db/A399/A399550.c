/*
 * A399550  Pascal's cube: entries in the 3-dimensional multivariate Pascal
 *          matrix, read by cubes, slices, and rows.
 * https://oeis.org/A399550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 274, u16 n <= 1487, u32 n <= 9546, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = binomial(m,i) * binomial(m,j) * binomial(m,k)
 * where
 *   (m*(m+1)/2)^2 <= n < ((m+1)*(m+2)/2)^2,
 *   q = n - (m*(m+1)/2)^2,
 *   k = floor(q / (m+1)^2),
 *   i = floor((q mod (m+1)^2) / (m+1)),
 *   j = q mod (m+1).
 * The function uses only integer arithmetic; no floating‑point is required.
 */

#include <stdint.h>

/* ---------- helper: binomial coefficient ---------- */
static seq_t binomial_calc(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = (result * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return result;
}

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    /* Find the cube index m such that (t_m)^2 <= n < (t_{m+1})^2
     * with t_m = m*(m+1)/2.
     */
    idx_t m = 0;
    idx_t t_m;
    idx_t t_m_sq;
    for (;;) {
        t_m = ((idx_t)m * (m + 1)) / 2;
        t_m_sq = t_m * t_m;
        if (t_m_sq > n) break;
        ++m;
    }
    --m;                                  /* step back to largest m with t_m^2 <= n */
    t_m = ((idx_t)m * (m + 1)) / 2;
    t_m_sq = t_m * t_m;

    /* Compute offsets q, k, i, j. */
    idx_t q = n - t_m_sq;
    idx_t mp1 = m + 1;
    idx_t mp1_sq = mp1 * mp1;
    idx_t k = q / mp1_sq;
    idx_t i = (q % mp1_sq) / mp1;
    idx_t j = q % mp1;

    /* Compute the product of binomials. */
    seq_t val = binomial_calc(m, i);
    val *= binomial_calc(m, j);
    val *= binomial_calc(m, k);
    return val;
}
