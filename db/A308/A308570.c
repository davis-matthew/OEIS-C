/*
 * A308570  a(n) = sigma_{2*n}(n).
 * https://oeis.org/A308570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma_{2*n}(n) = sum_{d|n} d^(2*n) (sigma_k(n) is the sum
 *            of the k-th powers of the divisors of n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp) */
#include "../utils/seq_mul.h"   /* seq_mul(seq_t a, seq_t b) */

/* Computes a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined for safety, though not used */

    seq_t result = 1;              /* accumulate product of local sums */
    idx_t to_factor = n;           /* local copy for factorization */

    /* Handle factor 2 separately for efficiency */
    if ((to_factor & 1) == 0) {
        idx_t exp = 0;
        while ((to_factor & 1) == 0) {
            to_factor >>= 1;
            exp++;
        }
        /* Compute sum_{i=0}^{exp} 2^(i*2*n) */
        seq_t pk = seq_pow((seq_t)2, 2 * n); /* 2^(2*n) */
        seq_t term = 1;
        seq_t local_sum = 0;
        for (idx_t i = 0; i <= exp; ++i) {
            local_sum += term;
            term = seq_mul(term, pk);
        }
        result = seq_mul(result, local_sum);
    }

    /* Factor odd divisors */
    for (idx_t p = 3; p * p <= to_factor; p += 2) {
        if (to_factor % p == 0) {
            idx_t exp = 0;
            while (to_factor % p == 0) {
                to_factor /= p;
                exp++;
            }
            seq_t pk = seq_pow((seq_t)p, 2 * n); /* p^(2*n) */
            seq_t term = 1;
            seq_t local_sum = 0;
            for (idx_t i = 0; i <= exp; ++i) {
                local_sum += term;              /* add p^(i*2*n) */
                term = seq_mul(term, pk);       /* advance to next power */
            }
            result = seq_mul(result, local_sum);
        }
    }

    /* If after division the remaining factor is > 1, it is prime */
    if (to_factor > 1) {
        seq_t pk = seq_pow((seq_t)to_factor, 2 * n); /* p^(2*n) */
        seq_t term = 1;
        seq_t local_sum = 0;
        for (idx_t i = 0; i <= 1; ++i) {   /* exponent is 1 */
            local_sum += term;
            term = seq_mul(term, pk);
        }
        result = seq_mul(result, local_sum);
    }

    return result;
}
