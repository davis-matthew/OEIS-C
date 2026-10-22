/*
 * A345890  a(n) = n + (n - 1) * (n - pi(n)).
 * https://oeis.org/A345890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + (n-1)*(n-pi(n))
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where pi(n) = number of primes ≤ n (OEIS A000720) */
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* a(1) is the first term of the sequence; behaviour for n<1 is irrelevant */
    seq_t pi_n = compute_pi(n);   /* pi(n) in seq_t */
    seq_t n_seq = (seq_t)n;       /* convert index to seq_t */
    seq_t one = (seq_t)1;

    /* Compute a(n) = n + (n-1)*(n-pi(n)) using only integer arithmetic */
    return n_seq + (n_seq - one) * (n_seq - pi_n);
}
