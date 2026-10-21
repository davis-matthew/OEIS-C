/*
 * A272630  a(n) = binomial(3*prime(n), prime(n)) - 3*binomial(2*prime(n),
 *          prime(n)) + 3.
 * https://oeis.org/A272630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(3*prime(n), prime(n)) - 3*binomial(2*prime(n),
 *            prime(n)) + 3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/gcd.h"

/* Compute binomial(n,k) exactly using a multiplicative algorithm.
   The result fits in seq_t. */
static seq_t binomial_exact(idx_t n, idx_t k)
{
    if (k > n) return 0u;
    if (k > n - k) k = n - k;          /* use symmetry */

    seq_t result = 1u;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* numerator term (n - k + i) and denominator term i */
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;

        /* cancel common factors between num/den and result */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        g = gcd(result, den);
        result /= g;
        den /= g;

        /* exact division: result is guaranteed to be a multiple of den */
        result = (result * num) / den;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0u) return 0u;           /* undefined for n < 1 */

    /* n-th prime (using 1‑based indexing) */
    idx_t p = get_prime(n);

    seq_t bin1 = binomial_exact(3u * p, p);  /* binomial(3p , p) */
    seq_t bin2 = binomial_exact(2u * p, p);  /* binomial(2p , p) */

    return bin1 - 3u * bin2 + 3u;
}
