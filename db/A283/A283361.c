/*
 * A283361  a(n) = (Product_{k=2..floor(2*n/3)} k^2) mod (2n-1).
 * https://oeis.org/A283361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=2..⌊2n/3⌋} k² (mod (2n-1))
 * Range:     u8 n <= 158, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(0)=0, a(1)=0, a(2)=1, …  (OEIS A283361) */

#include <stddef.h>   /* for NULL */

/* seq_t and idx_t are provided by the build system;
   assume seq_t supports basic arithmetic and % operator. */

seq_t a(idx_t n)
{
    /* indices 0 and 1 are defined to give 0 */
    if (n < 2) return 0;

    /* modulus m = 2n-1; for n>=2 this is ≥3 */
    seq_t m = (seq_t)(2 * n - 1);

    /* upper bound of the product range */
    idx_t maxk = (2 * n) / 3;            /* floor of 2n/3 */

    /* if maxk < 2 the product range is empty → result 1 (mod m) */
    if (maxk < 2) return 1 % m;          /* should be 1 for n=2 */

    seq_t prod = 1 % m;                  /* start with 1 modulo m */

    for (idx_t k = 2; k <= maxk; ++k)
    {
        seq_t kk = ((seq_t)k * (seq_t)k) % m;
        prod = (prod * kk) % m;
    }

    return prod;
}
