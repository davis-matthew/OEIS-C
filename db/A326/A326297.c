/*
 * A326297  If n = Product (p_j^k_j) then a(n) = Product ((p_j - 1)^(k_j -
 *          1)).
 * https://oeis.org/A326297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^k‖n} (p-1)^(k-1)
 * Range:     u8 n <= 3124, u16 n <= 8734, u32 n <= 8734, u64 n <= 8734,
 *            u128 n <= 8734, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for n ≥ 1.  n is given as an idx_t (uint64_t or similar).  */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t temp = (uint64_t)n;

    /* Trial division: factor n into prime powers */
    for (uint64_t p = 2;
         p * p <= temp;
         ++p)
    {
        if (temp % p == 0)
        {
            int exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }

            /* For exp > 1 add factor (p-1)^(exp-1) */
            if (exp > 1)
            {
                seq_t base = (seq_t)(p - 1);
                seq_t power = (seq_t)1;
                for (int i = 1; i < exp; ++i)
                    power *= base;        /* use built‑in multiplication on seq_t */

                result *= power;          /* multiply into running result */
            }
        }
    }

    /* Any remaining prime factor > sqrt(n) has exponent 1, so contributes nothing. */
    return result;
}
