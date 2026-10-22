/*
 * A360748  Expansion of Sum_{k>=0} (x * (1 + k*x^2))^k.
 * https://oeis.org/A360748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} (n-2k)^k * binomial(n-2k, k)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using the pre‑provided binomial function from utils/binomial.h. */
#include "../utils/binomial.h"

/* Compute x^e for integral exponent e using binary exponentiation.
   Only the allowed operators are used. */
static inline seq_t seq_pow(seq_t x, idx_t e)
{
    seq_t result = 1;
    seq_t base   = x;
    while (e > 0)
    {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t max_k = n / 3;
    for (idx_t k = 0; k <= max_k; ++k)
    {
        idx_t m   = n - 2 * k;          /* m = n-2k, guaranteed non‑negative */
        seq_t pow_term = seq_pow((seq_t)m, k);     /* (n-2k)^k  */
        seq_t bin_term = binomial(m, k);           /* binomial(n-2k,k) */
        total += pow_term * bin_term;
    }
    return total;
}
