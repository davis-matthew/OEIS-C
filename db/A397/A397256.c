/*
 * A397256  Square array T(n,k) = product of digits of k >= 1 in base n >= 2,
 *          read by falling antidiagonals.
 * https://oeis.org/A397256
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth entry of the array T(n,k) where T(n,k) = product
 *            of the base‑n digits of k (k≥1, n≥2), and the array is read by
 *            falling antidiagonals starting from (2,1). This implements the
 *            definition of OEIS sequence A397256
 * Range:     u8 n <= 8440, u16 n <= 8440, u32 n <= 8440, u64 n <= 8440,
 *            u128 n <= 8440, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* below offset – arbitrary */

    /* Zero‑based position in the enumeration, offset from first term a(2) */
    idx_t pos = n - 2;

    /* Find the diagonal sum S = n + k such that
       (S-2)(S-1)/2 > pos  and (S-3)(S-2)/2 ≤ pos. */
    idx_t S = 3;
    while ( (S - 2) * (S - 1) / 2 <= pos )
        ++S;

    idx_t prev = (S - 3) * (S - 2) / 2;   /* entries before diagonal S */
    idx_t offset = pos - prev;            /* offset within diagonal S */

    idx_t base = 2 + offset;              /* n value on this diagonal */
    idx_t k = S - base;                   /* corresponding k value, k≥1 */

    /* Compute product of base-n digits of k */
    seq_t prod = (seq_t)1;
    idx_t tmp = k;
    idx_t b = base;

    while (tmp > 0) {
        idx_t digit = tmp % b;
        prod *= (seq_t)digit;
        if (prod == 0) break;            /* early stop if zero encountered */
        tmp /= b;
    }
    return prod;
}
