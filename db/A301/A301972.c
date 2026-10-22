/*
 * A301972  a(n) = n*(n^2 - 2*n + 4)*binomial(2*n,n)/((n + 1)*(n + 2)).
 * https://oeis.org/A301972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n^2-2*n+4)*binomial(2*n,n)/((n + 1)*(n + 2))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t) n;                 /* index as seq_t */

    /* numerator factors */
    seq_t a_val = n_seq;
    seq_t b_val = n_seq * n_seq - n_seq * (seq_t)2 + (seq_t)4;
    seq_t c_val = binomial(2 * n, n);        /* choose(2n,n) */

    /* denominator factor */
    seq_t d_val = (n_seq + (seq_t)1) * (n_seq + (seq_t)2);

    /* cancel common divisors to keep intermediate values small */
    seq_t g;

    g = gcd(a_val, d_val);
    a_val /= g;  d_val /= g;

    g = gcd(b_val, d_val);
    b_val /= g;  d_val /= g;

    g = gcd(c_val, d_val);
    c_val /= g;  d_val /= g;

    /* d_val should now be 1 for exact division */
    /* multiply the remaining factors */
    seq_t result = a_val;
    result *= b_val;
    result *= c_val;

    return result;
}
