/*
 * A275944  Gaussian binomial coefficient [n,3] for q = 10.
 * https://oeis.org/A275944
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((10^n - 100) * (10^n - 10) * (10^n - 1)) / 890109000.
 *            The implementation divides out the denominator prime factors
 *            from the three numerators before multiplying, preventing
 *            overflow on 64‑bit builds while giving exact results for
 *            arbitrary‑precision builds
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0; /* below offset – undefined */

    /* 10^n */
    seq_t p10 = pow10(n);

    /* Numerators */
    seq_t nums[3];
    nums[0] = p10 - (seq_t)100;
    nums[1] = p10 - (seq_t)10;
    nums[2] = p10 - (seq_t)1;

    /* Denominator = 890109000 = 2^3 * 3^7 * 5^3 * 11 * 37 */
    seq_t denom = (seq_t)890109000;
    const seq_t primes[5] = { (seq_t)2, (seq_t)3, (seq_t)5, (seq_t)11, (seq_t)37 };
    const int  exps[5]   = { 3,    7,   3,    1,      1 };

    /* Cancel prime factors from the numerators */
    for (int pi = 0; pi < 5; ++pi) {
        seq_t p = primes[pi];
        while (denom % p == 0) {      /* while current prime still present in denom */
            bool divided = false;
            for (int j = 0; j < 3; ++j) {
                if (nums[j] % p == 0) {
                    nums[j] /= p;
                    denom /= p;
                    divided = true;
                    break;            /* go to next factor removal */
                }
            }
            if (!divided) break;      /* cannot cancel this prime further */
        }
    }

    /* Now denom should be 1 (for all valid n).  Multiply the remaining factors. */
    seq_t res = nums[0];
    res *= nums[1];
    res *= nums[2];

    /* In theory denom == 1 here. If not, divide any remaining factor. */
    if (denom != (seq_t)1) res /= denom;

    return res;
}
