/*
 * A359760  Triangle read by rows. The Kummer triangle, the coefficients of
 *          the Kummer polynomials. K(n, k) = binomial(n, k) *
 *          oddfactorial(k/2) if k is even, otherwise 0, where oddfactorial(z)
 *          := (2*z)!/(2^z*z!).
 * https://oeis.org/A359760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = K(r,k) where r = smallest integer with (r+1)(r+2)/2 > n,
 *            k = n - r(r+1)/2, and K(r,k) = 0 if k is odd, = binomial(r,k) *
 *            oddfact(k/2) otherwise. oddfact(m) = 1 * 3 * 5 * … * (2m-1) (by
 *            recurrence)
 * Range:     u8 n <= 39, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binomial, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Find the row r such that the flattened index n lies in row r. */
    idx_t r = 0;
    while ( ((r + 1) * (r + 2)) / 2 <= n )
        ++r;
    /* Sum of lengths of previous rows = r*(r+1)/2. */
    idx_t prev = r * (r + 1) / 2;
    idx_t k    = n - prev;          /* column within the row */
    if (k & 1U)                     /* odd k → coefficient is zero */
        return (seq_t)0;

    /* Ensure we have oddfact(k/2) in our cache. */
    static seq_t *oddfact = NULL;
    static size_t oddfact_len = 0;
    static size_t oddfact_cap = 0;
    idx_t z = k >> 1;                /* z = k/2 */

    if (oddfact_len == 0) {          /* first call – initialise cache */
        oddfact_cap = 1;
        oddfact_len = 1;
        oddfact = seq_alloc(oddfact_cap);
        oddfact[0] = (seq_t)1;
    }
    if (oddfact_len <= (size_t)z) {   /* need more entries */
        size_t new_cap = oddfact_cap;
        while (new_cap <= (size_t)z) new_cap = new_cap ? new_cap * 2 : 1;
        oddfact = seq_realloc(oddfact, oddfact_cap, new_cap);
        oddfact_cap = new_cap;
        while (oddfact_len <= (size_t)z) {
            seq_t prev_val = oddfact[oddfact_len - 1];
            seq_t mult = (seq_t)(2 * oddfact_len - 1);   /* odd factor to multiply */
            oddfact[oddfact_len] = prev_val * mult;
            ++oddfact_len;
        }
    }

    seq_t bin = binomial(r, k);      /* binomial coefficient */
    return bin * oddfact[z];         /* final Kummer number */
}
