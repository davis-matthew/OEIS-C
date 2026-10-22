/*
 * A335031  Complement of A334919.
 * https://oeis.org/A335031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer m such that 3*m+1 or (3*m+1)/2 is
 *            prime
 * Range:     u8 n <= 41, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Check if a given m satisfies the criterion */
static inline bool m_is_valid(seq_t m)
{
    seq_t t = 3 * m + 1;           /* compute 3*m+1 */
    if (is_prime_big(t))           /* if 3*m+1 is prime */
        return true;
    /* for odd m, (3*m+1)/2 is integer; check it */
    if ((m & 1u) && is_prime_big(t / 2))
        return true;
    return false;
}

/* Return the n‑th element of the sequence (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for n==0; return 0 as a safe default */
        return 0;

    idx_t count = 0;
    seq_t m = 1;

    while (true) {
        if (m_is_valid(m)) {
            ++count;
            if (count == n)
                return m;          /* found the n-th term */
        }
        ++m;
    }
}
