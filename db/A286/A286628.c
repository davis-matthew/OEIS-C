/*
 * A286628  a(n) = exponent of the highest power of A000005(n) (number of
 *          divisors of n) dividing A000203(n) (sum of divisors of n), a(1) =
 *          1.
 * https://oeis.org/A286628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exponent of τ(n) dividing σ(n), with a(1)=1
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here τ(n) = A000005(n) = number of divisors of n
 *      σ(n) = A000203(n) = sum of divisors of n
 * The return value is the largest k such that τ(n)^k | σ(n). */

#include <stdbool.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/sigma_of.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 1)          /* convention: a(1)=1 */
        return 1;

    /* τ(n) and σ(n) */
    seq_t tau = (seq_t)divisor_count_u64(n);
    seq_t s   = sigma_of(n);

    seq_t exp = 0;
    /* tau > 1 for all n > 1, so loop terminates naturally */
    while ((s % tau) == 0) {
        exp++;
        s /= tau;
    }
    return exp;
}
