/*
 * A389847  E.g.f. A(x) satisfies A(x) = exp(x^3 * A(x) / (1-x)^3).
 * https://oeis.org/A389847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (k+1)^(k-1) * binomial(n-1,
 *            n-3*k)/k!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"

/* fast integer exponentiation for seq_t */
static seq_t pow_seq_fast(seq_t base, int exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* compute n! */
    seq_t fact_n = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact_n *= (seq_t)i;

    seq_t result = 0;
    seq_t fact_k = 1;            /* k! */
    idx_t maxk = n / 3;          /* floor(n/3) */

    for (idx_t k = 1; k <= maxk; ++k) {
        fact_k *= (seq_t)k;      /* update k! */
        idx_t j = n - 3 * k;     /* n-3k */
        seq_t bin = binomial(n - 1, j);
        seq_t pow_val = pow_seq_fast((seq_t)(k + 1), (int)(k - 1));
        seq_t term = fact_n * bin * pow_val;
        term = term / fact_k;    /* exact division */
        result += term;
    }

    return result;
}
