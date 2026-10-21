/*
 * A293515  a(n) = Product_{d^k|n, d>1, k>1} prime(A286561(n,d)-1), where
 *          A286561(n,d) gives the highest exponent of d dividing n.
 * https://oeis.org/A293515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d^k|n, d>1, k>1} prime(A286561(n,d)-1)
 * Range:     u8 n <= 255, u16 n <= 5183, u32 n <= 9876, u64 n <= 9876,
 *            u128 n <= 9876, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here A286561(n,d) gives the largest exponent e such that d^e divides n */
/* The product is over all integers d>1 with d^2|n, and each factor is the prime with index (e-1). */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    seq_t result = 1;

    /* For n <= 1 the product is empty → 1. */
    /* Loop over all candidates d with d^2 ≤ n. */
    for (idx_t d = 2; d <= n / d; ++d) {
        /* d must divide n, and n/d must also be divisible by d for d^2|n. */
        if (n % d == 0) {
            idx_t n_div_d = n / d;
            if (n_div_d % d == 0) {
                /* Compute the maximal exponent e such that d^e | n. */
                idx_t temp = n;
                idx_t e = 0;
                while (temp % d == 0) {
                    temp /= d;
                    ++e;
                }
                /* e >= 2 because d^2 divides n. */
                if (e >= 2) {
                    /* Multiply by the prime with index (e-1). */
                    result *= (seq_t)get_prime(e - 1);
                }
            }
        }
    }
    return result;
}
