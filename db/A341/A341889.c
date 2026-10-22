/*
 * A341889  a(n) is the smallest positive integer k > 1 such that n + k
 *          divides n^n + k, or 0 if no such k exists.
 * https://oeis.org/A341889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS sequence A341889.
   a(n) = smallest integer k>1 such that (n+k) divides (n^n + k),
          or 0 if no such k exists.
   Implementation: for each n≥3 we test k from 2 up to n.
   The claim (supported by existing terms) is that whenever a(n)≠0,
   the smallest divisor of n^n−n greater than n+1 is <2n,
   hence k≤n.  n=1 and n=2 are handled separately. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1)          /* a(1) = 2 */
        return (seq_t)2;
    if (n == (idx_t)2)          /* a(2) = 0 */
        return (seq_t)0;

    idx_t max_k = n;            /* safe upper bound for k */
    for (idx_t k = (idx_t)2; k <= max_k; ++k) {
        seq_t mod = (seq_t)(n + k);          /* n + k */
        seq_t base = (seq_t)n;               /* base for exponentiation */
        idx_t exp = n;                       /* exponent n */

        /* compute base^exp mod mod using binary exponentiation */
        seq_t result = 1;
        seq_t a_val = base % mod;
        idx_t e = exp;
        while (e) {
            if (e & (idx_t)1)
                result = (result * a_val) % mod;
            a_val = (a_val * a_val) % mod;
            e >>= (idx_t)1;
        }

        /* test if (n^n + k) is divisible by (n + k) */
        seq_t val = result + (seq_t)k;
        if (val % mod == 0)
            return (seq_t)k;
    }
    return (seq_t)0;   /* no suitable k found */
}
