/*
 * A365878  Expansion of (1/x) * Series_Reversion( x*(1+x)^3*(1-x)^4 ).
 * https://oeis.org/A365878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..⌊n/2⌋} binomial(3*n + k + 2, k) *
 *            binomial(2*n - 2*k, n - 2*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute binomial coefficient C(n,k) using the multiplicative formula.
 * The function assumes idx_t fits the parameters n,k.  For k > n/2
 * it uses the symmetry property to reduce the number of loop iterations.
 */
static seq_t binom_multiplicative(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = (result * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Handle n = 0 separately to avoid division by zero in later formulas,
       although the generic code works as well. */
    if (n == 0) return 1;

    const idx_t max_k = n / 2;                     /* floor(n/2) */

    seq_t sum = 0;
    seq_t A   = 1;                                 /* binomial(3n+2,0) */
    seq_t B   = binom_multiplicative(2 * n, n);     /* binomial(2n,n) */

    for (idx_t k = 0; k <= max_k; ++k) {
        sum += A * B;

        if (k == max_k) break;                     /* finished loop */

        /* Update A:  A_{k+1} = A_k * (3n + k + 3) / (k + 1) */
        idx_t numA = 3 * n + k + 3;
        idx_t denA = k + 1;
        A = (A * (seq_t)numA) / (seq_t)denA;

        /* Update B:  B_{k+1} = B_k * (M_k*(M_k-1)) / (N_k*(N_k-1)) */
        /* where M_k = n - 2k   and   N_k = 2n - 2k. */
        idx_t M_k = n - 2 * k;
        idx_t N_k = 2 * n - 2 * k;

        seq_t numB = (seq_t)M_k * (M_k - 1);
        seq_t denB = (seq_t)N_k * (N_k - 1);
        B = (B * numB) / denB;
    }

    /* Final division by (n+1).  The result is guaranteed to be integral. */
    return sum / ((seq_t)n + 1);
}
