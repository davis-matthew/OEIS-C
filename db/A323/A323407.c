/*
 * A323407  Dirichlet inverse of A047994, unitary phi.
 * https://oeis.org/A323407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = multiplicative with a(p^e) = e*(1-p) for e <= 2, and
 *            a(p^e) = 2*a(p^(e-1)) - p*a(p^(e-2)) for e > 2
 * Range:     i8 n <= 130, i16 n <= 9959, i32 n <= 9959, i64 n <= 9959,
 *            i128 n <= 9959, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the value of a(p^e) according to the recurrence. */
static seq_t a_prime_power(seq_t p, idx_t e)
{
    if (e == 1)
        return (seq_t)1 * ((seq_t)1 - p);
    if (e == 2)
        return (seq_t)2 * ((seq_t)1 - p);

    seq_t prev2 = (seq_t)1 * ((seq_t)1 - p);   /* a(p^1) */
    seq_t prev1 = (seq_t)2 * ((seq_t)1 - p);   /* a(p^2) */
    seq_t cur = 0;

    for (idx_t k = 3; k <= e; ++k) {
        cur = (seq_t)2 * prev1 - p * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}

/*
 * Main function required by the test harness.
 * It evaluates a(n) for any n >= 1, using the multiplicative definition.
 */
seq_t a(idx_t n)
{
    /* n == 0 does not occur per problem statement. */
    seq_t result = 1;
    idx_t nn = n;

    /* Trial division factorisation – efficient for n up to about 10^5. */
    for (idx_t p = 2; p * p <= nn; ++p) {
        if (nn % p == 0) {
            idx_t e = 0;
            while (nn % p == 0) {
                nn /= p;
                ++e;
            }
            result *= a_prime_power((seq_t)p, e);
            /* If the factor returned zero, the product stays zero. */
            if (result == (seq_t)0)
                return (seq_t)0;
        }
    }

    /* Remaining prime factor, if any. */
    if (nn > 1) {
        result *= a_prime_power((seq_t)nn, (idx_t)1);
    }

    return result;
}
