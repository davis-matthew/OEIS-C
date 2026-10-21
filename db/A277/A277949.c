/*
 * A277949  Triangle read by rows, in which row n gives coefficients in
 *          expansion of ((x^n - 1)/(x - 1))^4.
 * https://oeis.org/A277949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = coefficient of x^k in ((x^n - 1)/(x - 1))^4, enumerated
 *            by flattening the triangular array T(n,k) defined in OEIS
 *            A277949. The function returns the n-th term of the flattened
 *            triangle, with offset 1 (a(1) = 1). See the OEIS entry for the
 *            piecewise definition of T(n,k)
 * Range:     u8 n <= 102, u16 n <= 4269, u32 n <= 7140, u64 n <= 7140,
 *            u128 n <= 7140, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return binomial coefficient C(n,3).  For n < 3 the result is 0. */
static seq_t binom3(idx_t n)
{
    if (n < 3) return (seq_t)0;
    /* Use a 128‑bit intermediate to avoid overflow. */
    unsigned __int128 t = (unsigned __int128)n * (n - 1) * (n - 2);
    t /= 6;
    return (seq_t)t;
}

/* Main sequence generator. */
seq_t a(idx_t n)
{
    /* The sequence is 1‑based.  For safety, return 0 for n == 0. */
    if (n == 0) return (seq_t)0;

    /* Find the row r such that 2*(r-1)^2 - (r-1) < n <= 2*r^2 - r. */
    idx_t r = 1;
    while ((2 * r * r - r) < n) ++r;

    /* Starting index of row r: 2*(r-1)^2 - (r-1) + 1. */
    idx_t start = 2 * (r - 1) * (r - 1) - (r - 1) + 1;

    /* Column index within the row (0‑based). */
    idx_t k = n - start;

    /* Compute T(r,k) using the piecewise formulas. */
    if (k < r) {
        /* 0 <= k < r: T = C(k+3,3) */
        return binom3(k + 3);
    } else if (k < 2 * r) {
        /* r <= k < 2*r: T = C(k+3,3) - 4*C(k-r+3,3) */
        seq_t a1 = binom3(k + 3);
        seq_t a2 = binom3(k - r + 3);
        return a1 - 4 * a2;
    } else if (k < 3 * r - 3) {
        /* 2*r-3 <= k < 3*r-3: T = C(4r-1-k,3) - 4*C(3r-1-k,3) */
        seq_t a1 = binom3(4 * r - 1 - k);
        seq_t a2 = binom3(3 * r - 1 - k);
        return a1 - 4 * a2;
    } else {
        /* 3*r-3 <= k < 4*r-3: T = C(4r-1-k,3) */
        return binom3(4 * r - 1 - k);
    }
}
