/*
 * A340180  a(n) = Sum_{x in C(n)} (sigma(n) mod x), where C(n) is the set of
 *          numbers < n coprime to n, and sigma = A000203.
 * https://oeis.org/A340180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{x in C(n)} (sigma(n) mod x)
 * Range:     u8 n <= 40, u16 n <= 604, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where C(n) = {x | 1 ≤ x < n and gcd(x,n)=1} and sigma(n) = sum of divisors of n */

#include <stddef.h>

static seq_t sum_of_divisors(idx_t n) {
    /* Compute σ(n) for a small n (≤ 10^6 suggested by timing limits). */
    if (n == 1) return (seq_t)1;
    seq_t sum = (seq_t)1;              /* include divisor 1 */
    for (idx_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t q = n / d;
            if (q != d) sum += (seq_t)q;
        }
    }
    sum += (seq_t)n;                   /* include divisor n itself */
    return sum;
}

static seq_t my_gcd(seq_t a, seq_t b) {
    /* Euclidean algorithm for generic integer type seq_t. */
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    /* Handle n=1 separately – there are no coprime numbers <1. */
    if (n <= 1) return (seq_t)0;

    seq_t sigma = sum_of_divisors(n);
    seq_t result = (seq_t)0;

    for (idx_t x = 1; x < n; ++x) {
        if (my_gcd((seq_t)x, (seq_t)n) == (seq_t)1) {
            result += sigma % (seq_t)x;
        }
    }
    return result;
}
