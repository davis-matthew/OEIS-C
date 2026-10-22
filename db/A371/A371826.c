/*
 * A371826  a(n) = Sum_{k=0..floor(n/2)} n^k * binomial(2*n-k,n-2*k).
 * https://oeis.org/A371826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A371826:
   a(n) = Sum_{k=0..⌊n/2⌋} n^k * binomial(2*n-k , n-2*k)   (offset 0) */

#include <stdint.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */
#include "../utils/seq_gcd.h"    /* provides seq_t seq_gcd(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t term = binomial(2*n, n);   /* k = 0 term */
    sum = term;

    idx_t maxk = n / 2;              /* number of transitions (k → k+1) */
    for (idx_t k = 0; k < maxk; ++k) {
        /* Compute term_{k+1} from term_k with exact cancellation to avoid
           intermediate overflow.  The formula is
               term_{k+1} = term_k * n*(n-2k)*(n-2k-1) / ((2n-k)*(n+k+1))   */
        seq_t res = term;

        /* numerator and denominator factors */
        seq_t num[3];
        seq_t den[2];

        num[0] = (seq_t)n;
        num[1] = (seq_t)(n - 2*k);
        num[2] = (seq_t)(n - 2*k - 1);

        den[0] = (seq_t)(2*n - k);
        den[1] = (seq_t)(n + k + 1);

        /* Cancel numerator factors with denominator factors */
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 2 && num[i] > 1; ++j) {
                seq_t g = seq_gcd(num[i], den[j]);
                if (g > (seq_t)1) {
                    num[i] /= g;
                    den[j] /= g;
                }
            }
            /* Cancel any remaining denominator factors against the current
               running result to keep the product small. */
            for (int j = 0; j < 2 && den[j] > (seq_t)1; ++j) {
                seq_t g = seq_gcd(res, den[j]);
                if (g > (seq_t)1) {
                    res /= g;
                    den[j] /= g;
                }
            }
            /* Multiply the remaining numerator factor */
            res *= num[i];
        }

        /* Any denominator factors left must be 1 (integer relation).  If
           not, divide them out from the result (this should never happen). */
        for (int j = 0; j < 2; ++j) {
            if (den[j] > (seq_t)1) {
                seq_t g = seq_gcd(res, den[j]);
                if (g > (seq_t)1) {
                    res /= g;
                    den[j] /= g;
                }
                if (den[j] != (seq_t)1)
                    res /= den[j];   /* safety */
            }
        }

        term = res;
        sum += term;
    }

    return sum;
}
