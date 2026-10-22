/*
 * A361902  Least k such that n+A000045(k) is prime, or -1 if no such k
 *          exists.
 * https://oeis.org/A361902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the least k such that n + F_k is prime; a(n) = 0 iff n
 *            is prime; a(n) = -1 if n ≡ 14475 (mod 1647030)
 * Range:     i8 n <= 84, i16 n <= 7122, i32 n <= 4982, i64 n <= 7122,
 *            i128 n <= 7122, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* Check trivial prime case */
    if (is_prime_big((seq_t)n))
        return (seq_t)0;

    /* Forbidden residue class -> no such k */
    const idx_t M = 1647030ULL;
    if (n % M == 14475ULL)
        return (seq_t)-1;

    /* Iterate Fibonacci indices starting from 1 */
    seq_t f_prev = 0; /* F_0 */
    seq_t f_curr = 1; /* F_1 */
    for (idx_t k = 1; ; ++k) {
        seq_t candidate = (seq_t)n + f_curr;
        if (is_prime_big(candidate))
            return (seq_t)k;
        /* Next Fibonacci number: F_{k+1} = F_k + F_{k-1} */
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }
}
