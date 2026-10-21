/*
 * A274978  Integers of the form m*(m + 6)/7.
 * https://oeis.org/A274978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*(n-1)*n - 5*(2*n-1)*(-1)^n - 5)/8
 * Range:     u8 n <= 12, u16 n <= 194, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast index to a wider unsigned integer for safe arithmetic. */
    unsigned long long NN = (unsigned long long)n;

    /* Compute the individual components of the numerator. */
    unsigned long long term1 = 14ULL * (NN - 1ULL) * NN;
    unsigned long long mult  = 5ULL * (2ULL * NN - 1ULL);

    /* Determine the sign of (-1)^n. */
    int sign = (NN & 1ULL) ? -1 : 1;

    unsigned long long numerator;
    if (sign == 1) {                     /* n even: subtract */
        numerator = term1 - mult - 5ULL;
    } else {                              /* n odd : add */
        numerator = term1 + mult - 5ULL;
    }

    /* Divide by 8 to obtain the term value. */
    unsigned long long result = numerator / 8ULL;

    return (seq_t)result;
}
