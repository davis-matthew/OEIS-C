/*
 * A382679  a(n) = A381968(A380817(n)).
 * https://oeis.org/A382679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A381968(A380817(n)) Computes the n‑th term of OEIS
 *            A382679. The sequence is formed from a triangular array: T(r,k)
 *            = A000384(r-1) + P(r,k), where A000384(m) = 2m² – m (square
 *            pyramidal numbers), and P(r,k) is given by piecewise rules in
 *            the OEIS entry. The length of row r is L(r) = 4r–3. T...
 * Range:     u8 n <= 231, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// Utility: integer square root (floor) for 64‑bit unsigned integers.
static idx_t isqrt_u64(idx_t v)
{
    idx_t res = 0;
    idx_t bit = 1ULL << 62;          /* largest power of 4 ≤ 2^64‑1 */

    /* shift bit down until it is ≤ v */
    while (bit > v) bit >>= 2;
    while (bit != 0) {
        if (v >= res + bit) {
            v -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* offset 1: n starts from 1 */
    if (n < 1) return 0;  /* undefined for n<1 */

    /* find row r such that S(r‑1) < n ≤ S(r), where S(r)=r(2r‑1) */
    idx_t r = (1 + isqrt_u64(8*n + 1)) / 4;   /* floor approximation */
    while (r*(2*r - 1) < n) r++;              /* ensure S(r) ≥ n */

    /* position k within row r (1‑based) */
    idx_t prevS = (r-1)*(2*(r-1) - 1); /* S(r‑1) */
    idx_t k = n - prevS;

    /* base value B = A000384(r‑1) = 2*(r-1)² – (r‑1) */
    seq_t r_minus = (seq_t)(r - 1);
    seq_t base = 2 * r_minus * r_minus - r_minus;

    /* compute P(r,k) */
    idx_t m = 2*r - 1;          /* m = 2r‑1 (odd) */
    idx_t P_idx;

    if (k < m) {
        if (k & 1) {            /* k odd */
            P_idx = 2*m - 1 - k;
        } else {                /* k even */
            P_idx = m + 1 - k;
        }
    } else {                    /* k ≥ m */
        if (k & 1) {            /* k odd */
            P_idx = k;
        } else {                /* k even */
            P_idx = 2*m - 1 - k;
        }
    }

    return base + (seq_t)P_idx;
}
