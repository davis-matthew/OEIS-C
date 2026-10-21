/*
 * A287865  a(n) = gpf(2*a(n-1)+1), with a(1)=1, where gpf = A006530.
 * https://oeis.org/A287865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(n)=gpf(2*a(n-1)+1) for n>1. The sequence becomes
 *            periodic with period 8 for n>9: a(n)=a(n-8). This function uses
 *            a small cache of the 8‑term cycle that starts with a(2)=3
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    /* static table for the 8‑term periodic cycle a(2)..a(9) */
    static seq_t cycle[8];
    static int initialized = 0;

    if (!initialized)
    {
        seq_t cur = (seq_t)1;   /* a(1) */
        for (int i = 0; i < 8; ++i)
        {
            /* compute next term: greatest prime factor of (2*cur + 1) */
            uint64_t temp = 2ULL * (uint64_t)cur + 1ULL;
            uint64_t gpf_val = gpf(temp);
            cur = (seq_t)gpf_val;
            cycle[i] = cur;     /* cycle[i] = a(2+i) */
        }
        initialized = 1;
    }

    if (n == 1ULL)
        return (seq_t)1ULL;

    /* For n >= 2: a(n) = cycle[(n-2) % 8];  indices 2..9 are the
       stored values; the period repeats thereafter. */
    return cycle[(n - 2ULL) % 8ULL];
}
