/*
 * A394352  a(n) is the least positive k such that n and 10^n - k are coprime.
 * https://oeis.org/A394352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   least k ≥ 1 such that gcd(n,10^n - k) = 1
 * Range:     u8 n <= 2102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset starts at 1, but guard anyway */

    seq_t mod = (seq_t)n;               /* modulus n */
    seq_t r   = mod_pow((seq_t)10, (uint64_t)n, mod);  /* 10^n mod n */

    seq_t x = r;
    for (idx_t k = 1; k <= n; ++k) {
        /* decrement x modulo n */
        if (x == 0) x = mod - 1;
        else        x = x - 1;

        if (gcd((seq_t)n, x) == 1)
            return (seq_t)k;
    }

    /* Should never reach here; fallback (defensive) */
    return (seq_t)n;
}
