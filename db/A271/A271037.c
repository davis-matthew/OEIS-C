/*
 * A271037  Number of 5X5X5 triangular 0..n arrays with some element less than
 *          a w, nw or ne neighbor exactly once.
 * https://oeis.org/A271037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{15} binom(n‑1, k) * Δ^k a(1) where Δ^k a(1) are
 *            the k‑th forward differences of the first 16 terms of the
 *            sequence
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Δ^k a(1) for k = 0 … 15 */
    static const seq_t delta[16] = {
        (seq_t)98,          /* Δ^0 a(1) */
        (seq_t)2322,        /* Δ^1 a(1) */
        (seq_t)24378,       /* Δ^2 a(1) */
        (seq_t)152234,      /* Δ^3 a(1) */
        (seq_t)637492,      /* Δ^4 a(1) */
        (seq_t)1907136,     /* Δ^5 a(1) */
        (seq_t)4228666,     /* Δ^6 a(1) */
        (seq_t)7099374,     /* Δ^7 a(1) */
        (seq_t)9122130,     /* Δ^8 a(1) */
        (seq_t)8989082,     /* Δ^9 a(1) */
        (seq_t)6748950,     /* Δ^10 a(1) */
        (seq_t)3796494,     /* Δ^11 a(1) */
        (seq_t)1550696,     /* Δ^12 a(1) */
        (seq_t)434596,      /* Δ^13 a(1) */
        (seq_t)74790,       /* Δ^14 a(1) */
        (seq_t)5962         /* Δ^15 a(1) */
    };

    if (n <= 0)
        return (seq_t)0;   /* out of offset, harmless */

    seq_t n1 = (seq_t)n - 1;
    seq_t sum = delta[0];
    seq_t binom = 1;  /* binom(n-1, 0) */

    for (idx_t k = 1; k < 16; ++k) {
        if (n1 < k - 1)
            break; /* binom = 0 for larger k */
        binom = binom * (n1 - (k - 1)) / (seq_t)k;
        sum += binom * delta[k];
    }
    return sum;
}
