/*
 * A340740  a(n) is the sum of all the remainders when n is divided by
 *          positive integers less than n/2 and coprime to n.
 * https://oeis.org/A340740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Σ_{k=1..⌊(n-1)/2⌋} (n mod k) * [gcd(n,k)=1]
 * Range:     u8 n <= 70, u16 n <= 1122, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the provided gcd(seq_t, seq_t).  */
/* Complexity: O(n log n) per call, fast enough for n ≤ 10 000.  */

#include <stdbool.h> /* for the bool type, if needed elsewhere */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* For n = 1 the loop below will not iterate, yielding 0 as required. */
    idx_t limit = (n - 1) / 2;        /* floor((n-1)/2) */
    seq_t total = 0;

    for (idx_t k = 1; k <= limit; ++k)
    {
        seq_t g = gcd((seq_t)n, (seq_t)k);  /* gcd(n,k) */
        if (g == (seq_t)1)                 /* coprime? */
        {
            /* n mod k fits into idx_t; cast to seq_t for accumulation */
            idx_t rem = n % k;
            total += (seq_t)rem;
        }
    }
    return total;
}
