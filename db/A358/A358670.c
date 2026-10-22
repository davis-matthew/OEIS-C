/*
 * A358670  a(n) = 1 if for all factorizations of n as x*y, the sum x+y is
 *          carryfree when the addition is done in the primorial base,
 *          otherwise 0.
 * https://oeis.org/A358670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1985, u16 n <= 9977, u32 n <= 9977, u64 n <= 9977,
 *            u128 n <= 9977, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A358670:
   a(n) = 1  if for all factorizations n = x*y, the addition x + y
   is carry‑free in primorial base; otherwise a(n) = 0.
*/
#include <stdbool.h>

static const uint64_t PRIMES[6] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL};
static const seq_t PRIMORD[6] = { (seq_t)1, (seq_t)2, (seq_t)6,
                                  (seq_t)30, (seq_t)210, (seq_t)2310 };

/* Test whether the sum of x and y has no carry when added in primorial base. */
static bool carryfree(seq_t x, seq_t y)
{
    for (int k = 0; k < 6; ++k) {
        seq_t base  = (seq_t)PRIMES[k];
        seq_t div   = PRIMORD[k];
        seq_t dx    = (x / div) % base;
        seq_t dy    = (y / div) % base;
        if (dx + dy >= base)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* offset starts at 1 */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t j = n / i;
            if (!carryfree((seq_t)i, (seq_t)j))
                return (seq_t)0;
        }
    }
    return (seq_t)1;
}
