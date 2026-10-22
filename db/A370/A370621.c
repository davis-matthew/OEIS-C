/*
 * A370621  Coefficient of x^n in the expansion of ( (1-x) / (1-x-x^2)^3 )^n.
 * https://oeis.org/A370621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(3*n + k - 1, k) *
 *            binomial(3*n - k - 1, n - 2*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    seq_t B1 = 1;
    seq_t B2 = binomial(3*n - 1, n);

    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        /* Add current term to the sum. */
        unsigned __int128 prod = (unsigned __int128)B1 * (unsigned __int128)B2;
        sum += (seq_t)prod;

        if (k == maxk)
            break;

        /* Update B1: B1_{k+1} = B1_k * (3*n + k) / (k+1). */
        seq_t num1 = (seq_t)(3*n + k);
        seq_t den1 = (seq_t)(k + 1);
        seq_t g1  = gcd(num1, den1);
        num1 /= g1;
        den1 /= g1;
        seq_t g2  = gcd(B1, den1);
        B1 /= g2;
        den1 /= g2;
        unsigned __int128 b1mul = (unsigned __int128)B1 * (unsigned __int128)num1;
        B1 = (seq_t)b1mul;

        /* Update B2: 
           B2_{k+1} = B2_k * K * (K-1) / ((2*n + k) * (3*n - k - 1)),
           where K = n - 2*k. */
        seq_t K = (seq_t)(n - 2*k);
        seq_t num2 = K * (K - 1);
        seq_t den2 = (seq_t)(2*n + k) * (seq_t)(3*n - k - 1);
        seq_t g3  = gcd(num2, den2);
        num2 /= g3;
        den2 /= g3;
        seq_t g4  = gcd(B2, den2);
        B2 /= g4;
        den2 /= g4;
        unsigned __int128 b2mul = (unsigned __int128)B2 * (unsigned __int128)num2;
        B2 = (seq_t)b2mul;
    }

    return sum;
}
