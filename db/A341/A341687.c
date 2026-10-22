/*
 * A341687  Expansion of the 7-adic integer Sum_{k>=0} k!.
 * https://oeis.org/A341687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (A341683(n+1) - A341683(n))/7^n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count7_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/count7_factors.h"

/* Compute the n‑th digit (base‑7) of the 7‑adic sum Σ k! . */
seq_t a(idx_t n)
{
    /* Number of base‑7 digits needed (0 … n). */
    size_t len = (size_t)n + 1;

    /* Allocate arrays for the running sum and the current factorial. */
    unsigned char *sum = (unsigned char *)malloc(len);
    unsigned char *fac = (unsigned char *)malloc(len);
    if (!sum || !fac) {
        /* Allocation failed: return 0 as a safe fallback. */
        free(sum);
        free(fac);
        return (seq_t)0;
    }

    /* Initialise arrays – all zero, then set 0! = 1 (digit 1 at position 0). */
    for (size_t i = 0; i < len; ++i) {
        sum[i] = 0;
        fac[i] = 0;
    }
    fac[0] = 1;
    sum[0] = 1;        /* add 0! */

    uint64_t val = 0;   /* valuation of current factorial (k!) */

    for (uint64_t k = 1; ; ++k) {
        /* Compute (k!) = (k-1)! * k  in base‑7, keeping only lower len digits. */
        unsigned int carry = 0;
        for (size_t i = 0; i < len; ++i) {
            unsigned int temp = (unsigned int)fac[i] * (unsigned int)k + carry;
            fac[i] = temp % 7;
            carry = temp / 7;
        }

        /* Update valuation of k! by counting factors of 7 in k. */
        val += count7_factors(k);
        if (val > n)
            break;              /* further terms would affect only higher digits */

        /* Add the new factorial to the running sum. */
        carry = 0;
        for (size_t i = 0; i < len; ++i) {
            unsigned int t = sum[i] + fac[i] + carry;
            sum[i] = t % 7;
            carry = t / 7;
        }
    }

    unsigned char result = sum[n];    /* digit in position n */
    free(sum);
    free(fac);
    return (seq_t)result;
}
