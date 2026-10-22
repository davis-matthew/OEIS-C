/*
 * A319697  Sum of even squarefree divisors of n.
 * https://oeis.org/A319697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n is odd; otherwise a(n) = 2 * ∏_{p|n, p odd prime}
 *            (1 + p)
 * Range:     u8 n <= 185, u16 n <= 9792, u32 n <= 9792, u64 n <= 9792,
 *            u128 n <= 9792, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* No even divisor for n < 2 or odd n */
    if (n < 2 || (n & 1ULL)) return 0;

    /* Remove all factors of 2 to obtain the odd part */
    idx_t odd_part = n;
    while ((odd_part & 1ULL) == 0) odd_part >>= 1;

    seq_t prod = 2;          /* the mandatory factor 2 from even divisors */

    /* Factor the odd part and multiply (p + 1) for each distinct odd prime divisor */
    for (idx_t p = 3; p <= odd_part / p; p += 2) {
        if (odd_part % p == 0) {
            prod *= (seq_t)(p + 1);
            while (odd_part % p == 0) odd_part /= p;  /* strip all powers of p */
        }
    }

    /* Remaining prime factor larger than sqrt, if any */
    if (odd_part > 1) {
        prod *= (seq_t)(odd_part + 1);
    }

    return prod;
}
