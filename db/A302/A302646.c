/*
 * A302646  Values of unimodal polynomial analogous to A302612 and A302644
 *          arising from a partition size <= 5 restriction.
 * https://oeis.org/A302646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 86, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{k=1}^{11} d_k * binom(n - k + 14, 14)
 *             d = [1, -13, 81, -315, 855, -1701, 2583, -2961, 2835, -1365, 2002]
 *
 * The formula follows from the given generating function
 *   x*(1 - 13*x + 81*x^2 - 315*x^3 + 855*x^4 - 1701*x^5 + 2583*x^6
 *    - 2961*x^7 + 2835*x^8 - 1365*x^9 + 2002*x^10) / (1 - x)^15 .
 *
 * The binomial coefficient is defined as binom(m,k)=0 for m<k.
 */

/* internal helper: compute binomial(m,k) using 128‑bit arithmetic when available */
#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 bigint_t;
#define BIGINT_T_SUPPORT
#else
typedef seq_t bigint_t;       /* fall back to seq_t; acceptable for large builds */
#endif

static inline seq_t binom(idx_t m, idx_t k) {
    if (k > m) return 0;
    if (k > m - k) k = m - k;           /* use symmetry */
    bigint_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (bigint_t)(m - k + i);   /* (m-k+i) ≤ m, fits in seq_t */
        res = res / (bigint_t)i;             /* division is exact */
    }
    return (seq_t)res;
}

/* sequence A302646 */
seq_t a(idx_t n) {
    /* offset 0: a(0)=0 */
    if (n == 0) return (seq_t)0;

    /* coefficients d_k for k=1..11 */
    static const int d[11] = {  1, -13,  81, -315,  855,
                              -1701, 2583, -2961, 2835, -1365,
                               2002 };

    bigint_t total = 0;
    for (int k = 1; k <= 11; ++k) {
        if (n < (idx_t)k) continue;                 /* binomial argument negative */
        idx_t m = n - k + 14;                       /* n-k+14 */
        seq_t bin = binom(m, 14);                   /* binomial coefficient */
        bigint_t term = (bigint_t)bin *
                        (bigint_t)(d[k-1] >= 0 ? d[k-1] : -d[k-1]);
        if (d[k-1] >= 0)
            total += term;
        else
            total -= term;
    }
    return (seq_t)total;
}
