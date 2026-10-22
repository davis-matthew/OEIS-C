/*
 * A375610  Expansion of e.g.f. 1 / (exp(-x) - x^3).
 * https://oeis.org/A375610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (k+1)^(n-3k)/(n-3k)!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t seq_pow(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n) {
    /* n = 0 is special: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    uint64_t max_k = n / 3;

    seq_t sum = (seq_t)1;   /* term for k = 0 */
    seq_t P   = (seq_t)1;   /* P = n! / (n - 3*k)! , start with k = 0 */

    for (uint64_t k = 1; k <= max_k; ++k) {
        /* multiply P by the next three descending factors */
        uint64_t start = n - 3 * (k - 1);
        P *= (seq_t)start;
        P *= (seq_t)(start - 1);
        P *= (seq_t)(start - 2);

        /* compute (k+1)^(n-3*k) */
        seq_t power = seq_pow((seq_t)(k + 1), n - 3 * k);

        /* add term to the sum */
        sum += P * power;
    }

    return sum;
}
