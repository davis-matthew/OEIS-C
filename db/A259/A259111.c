/*
 * A259111  a(n) = least number k > 1 such that 1^k + 2^k + ... + k^k == n
 *          (mod k).
 * https://oeis.org/A259111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k>1 with Σ_{i=1}^k i^k ≡ n (mod k)
 * Range:     u8 n <= 127, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sequence evaluator for A259111 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

static inline bool is_power_of_two(idx_t n)
{
    return n != 0 && (n & (n - 1)) == 0;
}

seq_t a(idx_t n)
{
    /* The sequence is defined only for n>=1; for n==0 we return 0 */
    if (n == 0) return (seq_t)0;

    /* Power‑of‑two case: a(2^m)=2^(m+1)=2*n */
    if (is_power_of_two(n))
        return (seq_t)(2 * n);

    /* Test k = 2; it works exactly when n is odd. */
    if ((n & 1) == 1)
        return (seq_t)2;

    /* Now n is even and not a power of two.
       For odd k the sum is always 0 (mod k), so we only need to
       check divisibility. For even k we must compute the sum. */
    for (idx_t k = 3; ; ++k)
    {
        if (k & 1)  /* odd k */
        {
            if ((n % k) == 0)
                return (seq_t)k;
        }
        else  /* even k */
        {
            seq_t sum = (seq_t)0;
            seq_t mod = (seq_t)k;
            for (idx_t i = 1; i <= k; ++i)
            {
                seq_t pw = mod_pow((seq_t)i, k, mod);
                sum += pw;
                sum %= mod;
            }
            if (sum == (seq_t)(n % k))
                return (seq_t)k;
        }
    }
}
