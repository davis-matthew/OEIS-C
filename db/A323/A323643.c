/*
 * A323643  a(n) is the sum of the noncentral divisors of n.
 * https://oeis.org/A323643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = sum of noncentral divisors of n Computation: for each
 *            divisor d ≤ √n add d and n/d to the divisor sum, keep track of
 *            the two middle divisors (the largest divisor ≤ √n and its
 *            co‑divisor). Subtract the middle divisor(s) from the total sum
 * Range:     u8 n <= 107, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static idx_t int_sqrt_u64(idx_t v)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= v)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* The sum of all divisors σ(n) */
    seq_t sigma = 0;
    /* Track the two middle divisors (largest divisor ≤ √n and its co‑divisor) */
    idx_t mid_d1 = 1, mid_d2 = n;          /* initialized to trivial pair */

    idx_t root = int_sqrt_u64(n);
    for (idx_t d = 1; d <= root; ++d)
    {
        if (n % d == 0)
        {
            idx_t d2 = n / d;
            sigma += (seq_t)d;
            if (d2 != d)
                sigma += (seq_t)d2;

            /* Update the middle divisor pair (the last divisor ≤ √n will be stored) */
            mid_d1 = d;
            mid_d2 = d2;
        }
    }

    /* Compute the sum of the central divisor(s) */
    seq_t central = (mid_d1 == mid_d2) ? (seq_t)mid_d1 : ((seq_t)mid_d1 + (seq_t)mid_d2);

    return sigma - central;
}
