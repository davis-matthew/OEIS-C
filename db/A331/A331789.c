/*
 * A331789  T(b,n) is the smallest m such that for any N, at least one of
 *          S(N), S(N+1), ..., S(N+m-1) is divisible by n, where S(N) is the
 *          sum of digits of N in base b. Square array read by ascending
 *          antidiagonals.
 * https://oeis.org/A331789
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, easy, tabl
 * Formula:   a(n) is the value of T(b,n) where (b,n) are obtained from the
 *            OEIS anti‑diagonal index n (offset 2). T(b,n) is given by T(b,n)
 *            = b^s * (2*t - gcd(t,b-1) + 1) - 1, where n = (b-1)*s + t with 1
 *            ≤ t ≤ b-1. The map from index n to (b,n) is: - Let k = n-2
 *            (zero‑based). - Find the smallest d ≥ 3 such t...
 * Range:     u8 n <= 45, u16 n <= 153, u32 n <= 561, u64 n <= 2145,
 *            u128 n <= 7627, big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n >= 2 due to offset.  For indices below 2 the behaviour is
       unspecified and the function is not called. */
    idx_t k = n - 2;               /* zero‑based index of the element   */

    /* Locate the diagonal d such that
       cumulative_terms(d-1) ≤ k < cumulative_terms(d)  ,
       where cumulative_terms(d) = (d-2)*(d-1)/2.           */
    idx_t d = 3;      /* first diagonal (sum = 3) has one element */
    idx_t cum = 1;    /* cumulative_terms(3) */

    while (cum <= k) {
        ++d;
        cum += d - 2;  /* add the length of the new diagonal */
    }

    idx_t prev_cum = cum - (d - 2);
    idx_t offset = k - prev_cum;      /* position inside the diagonal   */

    /* Compute (b, n_pair) from diagonal d and offset.              */
    idx_t b = (d - 1) - offset;        /* b >= 2                          */
    idx_t n_pair = offset + 1;          /* n_pair >= 1                     */

    /* Compute s and t:  n_pair = (b-1)*s + t , 1 ≤ t ≤ b-1          */
    idx_t s = (n_pair - 1) / (b - 1);
    idx_t t = n_pair - s * (b - 1);

    /* Convert to seq_t for arithmetic                     */
    seq_t b_seq     = (seq_t)b;
    seq_t b_minus1  = (seq_t)(b - 1);
    seq_t t_seq     = (seq_t)t;

    seq_t gcd_val   = gcd(t_seq, b_minus1);
    seq_t num_factor = (seq_t)2 * t_seq - gcd_val + (seq_t)1;

    seq_t pow_val   = big_pow(b_seq, (seq_t)s);

    seq_t result = pow_val * num_factor - (seq_t)1;
    return result;
}
