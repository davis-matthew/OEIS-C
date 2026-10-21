/*
 * A247490  Square array read by antidiagonals: A(k, n) = (-1)^(n+1)*
 *          hypergeom([k, -n+1], [], 1) for n>0 and A(k,0) = 0 (n>=0, k>=1).
 * https://oeis.org/A247490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 51, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A247490
 * Defined by the square array A(k,n) = (-1)^(n+1) * ₂F₀(k,1-n;;1) for n>0 and A(k,0)=0.
 * The sequence a(n) is read along antidiagonals (k+n constant, n increasing).
 * Algorithm:
 *   1. Map the global index n to coordinates (k,n₁) in the array.
 *   2. Compute A(k,n₁) by an iterative hypergeometric series that uses only
 *      integer arithmetic (no floating point, no libraries).
 * Complexity: O(n₁) where n₁ ≤ √(2n) – far below the time limits. */

seq_t a(idx_t n)
{
    /* --- 1.  Find diagonal number t (k + n₁)  --- */
    idx_t low  = 0;
    idx_t high = n;                     /* high is inclusive bounds for mid */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        /* Mid*(mid+1)/2 is the number of elements before diagonal mid+1 */
        __uint128_t prod = (__uint128_t)mid * (mid + 1) / 2;
        if (prod <= n) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    idx_t t = low + 1;                                   /* diagonal number */
    __uint128_t before = (__uint128_t)low * (low + 1) / 2; /* elements before diagonal t */
    idx_t d     = n - (idx_t)before;                     /* offset inside the diagonal */
    idx_t n₁    = d;                                     /* column index in the array */
    idx_t k₁    = t - d;                                 /* row index in the array */

    /* --- 2.  Compute A(k₁,n₁)  --- */
    if (n₁ == 0) {
        /* By definition A(k,0) = 0. */
        return 0;
    }

    seq_t pos = 0;           /* sum of terms with even (n₁+1+j) parity   */
    seq_t neg = 0;           /* sum of terms with odd  (n₁+1+j) parity   */
    seq_t term = 1;          /* current term (k₁)_j * (n₁-1)_j / j! */

    for (idx_t j = 0; j < n₁; ++j) {
        /* Add the current term with the proper sign. */
        if (((n₁ + 1 + j) & 1) == 0) {
            pos += term;
        } else {
            neg += term;
        }
        /* Prepare next term if needed. */
        if (j + 1 < n₁) {
            term = term * (seq_t)(k₁ + j);   /* multiply by (k₁ + j) */
            term = term * (seq_t)(n₁ - j - 1);/* multiply by (n₁ - j - 1) */
            term = term / (j + 1);           /* divide by (j+1) – exact division */
        }
    }

    /* The final value is the difference of the two partial sums. */
    seq_t result = pos - neg;
    return result;
}
