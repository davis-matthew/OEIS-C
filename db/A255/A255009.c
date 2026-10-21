/*
 * A255009  Array T(n,k) read by ascending antidiagonals, where T(n,k) is the
 *          denominator of polygamma(n, 1) - polygamma(n, k).
 * https://oeis.org/A255009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 30, u32 n <= 49, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   antidiag_index, factorial, seq_gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A255009
    a(n) is the denominator of polygamma(r,1) - polygamma(r,k)
    where the pair (r,k) is obtained by reading the array
    T(r,k)  (r ≥ 0, k ≥ 1) by ascending antidiagonals
    and ascending k within each antidiagonal.  */
#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp)   */
#include "../utils/seq_gcd.h"   /* seq_gcd(seq_t a, seq_t b)       */
#include "../utils/factorial.h" /* factorial(idx_t n)               */
#include "../utils/antidiag_index.h"

/*  Compute a(n) for the OEIS A255009 sequence.   */
seq_t a(idx_t n) {
    /* Determine the corresponding (r,k) pair.   */
    idx_t r, k;
    antidiag_index(n, &r, &k);

    /* Special case k==1 → the difference is zero, denominator 1.  */
    if (k <= 1)
        return (seq_t)1;

    /* Helper: compute the denominator of H_{k-1}^{(r+1)}   */
    seq_t H_num = 0;  /* numerator */
    seq_t H_den = 1;  /* denominator */

    /* Accumulate the sum Σ_{i=1}^{k-1} 1 / i^{r+1} in rational form.  */
    idx_t exp = r + 1;
    for (idx_t i = 1; i < k; ++i) {
        seq_t term_den = seq_pow((seq_t)i, exp);          /* i^{r+1}  */
        seq_t g1 = seq_gcd(H_den, term_den);
        seq_t lcm = (H_den / g1) * term_den;              /* lcm(den, term_den) */

        seq_t num1 = H_num * (lcm / H_den);
        seq_t num2 = (seq_t)1 * (lcm / term_den);
        seq_t new_num = num1 + num2;                      /* new numerator */

        seq_t g2 = seq_gcd(new_num, lcm);                 /* reduce fraction */
        H_num = new_num / g2;
        H_den = lcm / g2;
    }

    /* Multiply by r! (n!) to obtain the exact rational value.   */
    seq_t nfac = factorial(r);
    seq_t num = nfac * H_num;

    /* Reduce fraction num / H_den and return the denominator.   */
    seq_t g = seq_gcd(num, H_den);
    return H_den / g;
}
