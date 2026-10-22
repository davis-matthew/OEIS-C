/*
 * A365735  G.f. satisfies A(x) = 1 + x*A(x) / (1 - x^5*A(x)^3).
 * https://oeis.org/A365735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 34, u64 n <= 63, u128 n <= 119,
 *            big any n
 * Helpers:   binomial, gcd, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A365735
   a(n) = Σ_{k=0..⌊n/5⌋} binomial(n-4k-1 , k) *
                binomial(n-2k+1 , n-5k) / (n-2k+1)
   Offset: 0  (a(0)=1)                                            */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* handle the trivial case a(0)=1 without entering the sum */
    if (n == 0)
        return (seq_t)1;

    seq_t total = 0;
    idx_t maxk = n / 5;          /* floor(n/5) */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* --- first binomial: binomial(n-4k-1 , k) --- */
        seq_t bin1;
        if (k == 0) {
            /* binomial(-1,0) is 1; special case for n=0 handled above */
            bin1 = (seq_t)1;
        } else if (n >= 4 * k + 1) {
            bin1 = binomial(n - 4 * k - 1, k);
        } else {
            bin1 = 0;
        }

        /* --- second binomial: binomial(n-2k+1 , n-5k) --- */
        seq_t bin2;
        if (k == 0) {
            bin2 = binomial(n + 1, n);          /* binomial(n+1,n) = 1 */
        } else if (n >= 5 * k) {
            bin2 = binomial(n - 2 * k + 1, n - 5 * k);
        } else {
            bin2 = 0;
        }

        /* --- divide by (n-2k+1) while avoiding overflow --- */
        seq_t denom = (seq_t)(n) - 2 * (seq_t)k + 1;  /* n-2k+1 */
        if (denom == 0)                     /* safety, should not happen */
            continue;

        /* Reduce bin1 and bin2 with denom using gcd before multiplication */
        seq_t g = gcd(bin1, denom);
        bin1 /= g;
        denom /= g;

        g = gcd(bin2, denom);
        bin2 /= g;
        denom /= g;

        /* After reduction denom must be 1 (exact division).  If not,
           the formula guarantees divisibility, but for safety we ignore
           the remaining part. */
        seq_t term = seq_mul(bin1, bin2);
        if (denom != 1)
            term /= denom;     /* this case should not happen */

        total += term;
    }

    return total;
}
