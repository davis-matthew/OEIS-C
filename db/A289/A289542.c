/*
 * A289542  Number of ordered pairs of nonzero vectors over the subspaces of
 *          GF(2)^n.
 * https://oeis.org/A289542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n} binom_q(n,k)* (2^k-1)^2 with q=2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here binom_q(n,k) = [n choose k]_2 is the Gaussian binomial coefficient. */
/* We compute it using the multiplicative recurrence
 *   G_0 = 1
 *   G_k = G_{k-1} * (2^{n-k+1}-1) / (2^k-1)
 * where G_k = [n choose k]_2. */

#include <stdbool.h>
#include <stddef.h>

/* Function prototypes for helper operations are provided by the build system. */
extern seq_t seq_pow(seq_t base, idx_t exp);   /* not used, but line keeps consistency */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    seq_t sum = (seq_t)0;
    seq_t G = (seq_t)1;           /* G_0 = 1 */
    seq_t pow2_k = (seq_t)1;      /* will hold 2^k */
    seq_t pow2_nk = (seq_t)1;     /* will hold 2^n initially */

    /* compute 2^n into pow2_nk */
    for (idx_t i = 0; i < n; ++i) {
        pow2_nk <<= 1;
    }

    for (idx_t k = 1; k <= n; ++k) {
        /* update 2^k */
        pow2_k <<= 1;                      /* pow2_k = 2^k */
        seq_t two_minus_k = pow2_k - (seq_t)1;   /* 2^k - 1 */

        /* numerator factor: 2^{n-k+1} - 1 */
        seq_t numerator = pow2_nk - (seq_t)1;

        /* Update G to [n choose k]_2 */
        G = (G * numerator) / two_minus_k;

        /* term = G * (2^k-1)^2 */
        seq_t tmp = two_minus_k * two_minus_k;   /* (2^k-1)^2 */
        sum += G * tmp;

        /* prepare pow2_nk for next k: shift right by 1 */
        pow2_nk >>= 1;
    }

    return sum;
}
