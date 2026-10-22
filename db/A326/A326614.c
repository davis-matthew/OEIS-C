/*
 * A326614  Smallest Euler-Jacobi pseudoprime to base n.
 * https://oeis.org/A326614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   gcd, isPrimeTrial, jacobi_symbol, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest Euler–Jacobi pseudoprime to base n:
   a(n) = smallest odd composite m such that
   gcd(n,m)=1 and n^((m‑1)/2) ≡ (n|m) (mod m). */
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/isPrimeTrial.h"
#include "../utils/jacobi_symbol.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    uint64_t m = 3;                      /* start with the first odd >1   */
    for (;; m += 2) {                    /* only odd numbers are checked  */
        if (isPrimeTrial((seq_t)m))      /* skip primes                    */
            continue;
        seq_t g = gcd((seq_t)n, (seq_t)m);
        if (g != (seq_t)1)               /* require coprimality            */
            continue;
        idx_t exp = (idx_t)((m - 1) / 2);
        seq_t power = mod_pow((seq_t)n, exp, (seq_t)m);
        int jsym = jacobi_symbol((uint64_t)n, (uint64_t)m);
        seq_t want = (jsym == -1) ? ((seq_t)m - 1) : (seq_t)1;
        if (power == want)              /* pseudoprime condition met      */
            return (seq_t)m;            /* m is the smallest such value   */
    }
}
