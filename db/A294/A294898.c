/*
 * A294898  Deficiency minus binary weight: a(n) = A033879(n) - A000120(n) =
 *          A005187(n) - A000203(n).
 * https://oeis.org/A294898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*n - sigma(n) - popcount(n) where sigma(n) = sum of
 *            divisors of n. We compute sigma by prime factorisation using
 *            trial division
 * Range:     i8 n <= 136, i16 n <= 8980, i32 n <= 8980, i64 n <= 8980,
 *            i128 n <= 8980, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t sigma_of(idx_t n)
{
    if (n == 0) return 0; /* Not used, but keep safe */
    seq_t result = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            seq_t sum_p = 1;      /* sum of powers of p: 1 + p + p^2 + ... */
            seq_t p_pow = 1;      /* current p^k */
            while (m % p == 0) {
                m /= p;
                p_pow *= (seq_t)p;
                sum_p += p_pow;
            }
            result *= sum_p;
        }
    }
    if (m > 1) {                /* remaining prime factor */
        result *= (seq_t)(1 + m);
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety: sequence defined for n>=1 */
    seq_t n_seq = (seq_t)n;
    seq_t sigma = sigma_of(n);
    /* popcount of n using builtin __builtin_popcountll (works for 64-bit) */
    seq_t pop = (seq_t)__builtin_popcountll(n);
    return 2 * n_seq - sigma - pop;
}
