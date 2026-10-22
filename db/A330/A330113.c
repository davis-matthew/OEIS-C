/*
 * A330113  Beatty sequence for cosh(x), where 1/e^x + sech(x) = 1.
 * https://oeis.org/A330113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * α), where α is the unique real root of 2α³ –
 *            4α² + 2α – 1 = 0 (α ≈ 1.56459…). The function evaluates the
 *            floor by a binary search using only integer arithmetic
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not defined for n < 1 */

    /* Search interval: α ∈ (1, 2), so floor(n·α) ∈ [n, 2n] */
    idx_t low  = n;
    idx_t high = n;
    high += n;               /* high = 2*n (overflow improbable for intended use) */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;

        /* Convert to seq_t for high‑precision arithmetic */
        seq_t m = (seq_t)mid;
        seq_t nn = (seq_t)n;

        /* Compute m², m³, n², n³ */
        seq_t m2 = m * m;
        seq_t m3 = m2 * m;
        seq_t n2 = nn * nn;
        seq_t n3 = n2 * nn;

        /* lhs = 2·m³ */
        seq_t lhs = m3 + m3;

        /* rhs = 4·m²·n – 2·m·n² + n³ */
        seq_t t1 = m2 * nn;          /* m²·n */
        t1 = t1 + t1 + t1 + t1;      /* 4·m²·n */
        seq_t t2 = m * n2;           /* m·n² */
        t2 = t2 + t2;                /* 2·m·n² */
        seq_t rhs = t1 + n3;
        rhs -= t2;

        /* If lhs > rhs, α > mid/n → target lies below mid */
        if (lhs > rhs)
            high = mid;
        else
            low = mid + 1;
    }

    /* low is the first m with lhs > rhs; floor(n·α) = low – 1 */
    return low - 1;
}
