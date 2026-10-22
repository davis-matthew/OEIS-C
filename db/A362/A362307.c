/*
 * A362307  Row sums of A362370.
 * https://oeis.org/A362307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} floor(binomial(n - 1, k - 1) / k!)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 36, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 as defined in the OEIS entry. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* We work with N = n - 1 so that C(N, r) = binomial(n - 1, r). */
    const idx_t N = n - 1;

    seq_t sum   = (seq_t)0;
    seq_t C     = (seq_t)1;  /* C(N, 0) */
    seq_t term  = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k) {
        /* Compute term = floor( C(N, k-1) / k! ).
           We perform the division by successively dividing by
           2, 3, ..., k.  This avoids keeping k! in a separate
           big integer and prevents overflow on fixed‐width
           seq_t types. */
        term = C;
        for (idx_t d = 2; d <= k; ++d) {
            term /= (seq_t)d;
        }

        sum += term;

        /* Termination: once term becomes zero, all subsequent
           terms will stay zero because k! grows faster than the
           remaining binomial coefficients. */
        if (term == 0) {
            break;
        }

        /* Prepare C for the next k:
           C(N, k) = C(N, k-1) * (N - (k-1)) / k   (exact division). */
        if (k < n) {
            C = (C * (seq_t)(N - (k - 1))) / (seq_t)k;
        }
    }

    return sum;
}
