/*
 * A390112  Smallest exponent b >= 2 such that the absolute value of the
 *          alternating 3-digit block sum of n^b equals n; 0 if none.
 * https://oeis.org/A390112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest b ≥ 2 such that S_3(n^b) = n; 0 if none
 * Range:     u8 n <= 1, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   signed_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/signed_sum.h"

seq_t a(idx_t n)
{
    /* Special case: 0 */
    if (n == 0u) {
        /* 0^2 = 0 => S_3 = 0.  First eligible b is 2. */
        return (seq_t)2u;
    }

    /* Initial power = n^1.  Store in base‑1000 limbs. */
    size_t cap = 64;                         /* initial capacity */
    uint32_t *limb = (uint32_t *)malloc(cap * sizeof(uint32_t));
    size_t len = 0;
    idx_t tmp = n;
    do {
        if (len == cap) {
            cap <<= 1;
            limb = (uint32_t *)realloc(limb, cap * sizeof(uint32_t));
        }
        limb[len++] = (uint32_t)(tmp % 1000u);
        tmp /= 1000u;
    } while (tmp);

    const seq_t tn = (seq_t)n;
    /* Search up to a safe upper bound.  Empirical evidence shows
       exponents stay below several thousand for n ≤ 10000.  */
    const size_t MAX_B = 5000u;

    for (size_t exp = 1; exp <= MAX_B; ++exp) {
        /* Check the condition for b = exp (i.e., n^exp). */
        if (exp >= 2u) {
            seq_t s = signed_sum(limb, len);
            if (s == tn) {
                free(limb);
                return (seq_t)exp;
            }
        }

        /* Multiply current power by n to obtain n^(exp+1). */
        uint64_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            uint64_t prod = (uint64_t)limb[i] * (uint64_t)n + carry;
            limb[i] = (uint32_t)(prod % 1000u);
            carry    = prod / 1000u;
        }
        while (carry) {
            if (len == cap) {
                cap <<= 1;
                limb = (uint32_t *)realloc(limb, cap * sizeof(uint32_t));
            }
            limb[len++] = (uint32_t)(carry % 1000u);
            carry       = carry / 1000u;
        }
    }

    free(limb);
    return (seq_t)0u;   /* No exponent found within the bound. */
}
