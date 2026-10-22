/*
 * A389588  Number of integer lattice points (x,y) with x^2 + y^2 <= n^2 that
 *          lie outside the interior of the regular hexagon inscribed in the
 *          circle of radius n centered at the origin.
 * https://oeis.org/A389588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000328(n) - I(n), where I(n) = 2*n - 2*m - 1 + 4*(m*n -
 *            Σ_{k=1..m} ⌈n - k/√3⌉), m = ⌊√3*n/2⌋. The implementation uses
 *            only integer arithmetic
 * Range:     u8 n <= 4, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for the offset, but return 0 */

    seq_t sn      = (seq_t)n;
    seq_t n_sq    = sn * sn;           /* n² */

    /* Circle count A000328(n) = 4*Σ_{x=1..n}⌊√(n²-x²)⌋ + 4n + 1 */
    seq_t sum_sqrt = 0;
    for (idx_t x = 1; x <= n; ++x) {
        seq_t x_sq = (seq_t)x * (seq_t)x;
        seq_t d    = n_sq - x_sq;          /* n² - x² ≥ 0 */
        seq_t t    = floor_sqrt(d);        /* ⌊√(n²-x²)⌋ */
        sum_sqrt += t;
    }
    seq_t circ = (seq_t)4 * sum_sqrt + (seq_t)4 * sn + (seq_t)1;

    /* Compute m = ⌊√(3) * n / 2⌋ using integer arithmetic:
       m = ⌊√(3*n²)⌋ / 2                                   */
    seq_t sqrt3n_sq = floor_sqrt((seq_t)3 * n_sq);  /* ⌊√(3*n²)⌋ */
    seq_t m_seq     = sqrt3n_sq / 2;
    idx_t  m        = (idx_t)m_seq;                 /* m fits into idx_t for tests */

    /* Sum of ⌊k/√3⌋ for k = 1..m.
       ⌊k/√3⌋ = ⌊√(k²/3)⌋; we use this integer identity. */
    seq_t sum_floors = 0;
    for (idx_t k = 1; k <= m; ++k) {
        seq_t k_sq        = (seq_t)k * (seq_t)k;
        seq_t k_sq_div3   = k_sq / 3;                /* floor(k²/3) */
        seq_t fk          = floor_sqrt(k_sq_div3);   /* ⌊√(k²/3)⌋ = ⌊k/√3⌋ */
        sum_floors += fk;
    }

    /* Interior lattice points strictly inside the hexagon */
    seq_t I = (seq_t)4 * m_seq * sn
              + (seq_t)2 * sn
              - (seq_t)2 * m_seq
              - (seq_t)1
              - (seq_t)4 * sum_floors;

    /* Final sequence value */
    return circ - I;
}
