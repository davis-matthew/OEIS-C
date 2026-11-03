/*
 * A306762  Smallest integer k such that Sum_(i=1..k) lambda(i) is divisible
 *          by n, where lambda(i) is the Carmichael lambda function.
 * https://oeis.org/A306762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-01)
 * Helpers:   gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A306762:
   a(n) = the smallest integer k such that
          Sum_{i=1..k} λ(i)  is divisible by n
   where λ(i) is the Carmichael lambda function. */

/* Required types are provided by the build environment:
   seq_t  – unsigned integer type for sequence values
   idx_t  – unsigned 64‑bit integer type for indices   */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd_idx.h"

/* Compute Carmichael λ(m) for 1 ≤ m ≤ UINT64_MAX.  Return a 64‑bit
   value; for the purposes of this sequence we need only the value
   modulo n, so the caller may take the result modulo n if desired. */
static idx_t _lambda_value(idx_t m)
{
    if (m == 1) return 1;          /* by convention λ(1)=1 */
    idx_t res = 1;
    idx_t x = m;

    /* handle factor 2 */
    if ((x & 1) == 0) {
        int e = 0;
        while ((x & 1) == 0) {
            x >>= 1;
            ++e;
        }
        idx_t l;
        if (e == 1)        l = 1;
        else if (e == 2)   l = 2;
        else               l = ((idx_t)1) << (e - 2);   /* 2^(e-2) */
        res = l;           /* lcm(1,l)=l */
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            int e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            /* λ(p^e) = (p-1) * p^(e-1) for odd p */
            idx_t pow = 1;
            for (int i = 1; i < e; ++i) pow *= p; /* p^(e-1) */
            idx_t l = (p - 1) * pow;
            idx_t g = gcd_idx(res, l);
            res = (res / g) * l;
        }
    }

    /* remaining prime factor */
    if (x > 1) {
        /* x is prime */
        idx_t l = (x - 1);
        idx_t g = gcd_idx(res, l);
        res = (res / g) * l;
    }

    return res;
}

/* ------------------------------------------------------------------ */
/* Euclidean GCD for idx_t.                                          */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Public sequence function.                                          */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined for n=0, but defensive */

    idx_t sum_mod = 0;       /* running sum modulo n          */
    idx_t k = 0;             /* candidate k                    */

    while (true) {
        ++k;
        idx_t lambda_k = _lambda_value(k);
        sum_mod = (sum_mod + (lambda_k % n)) % n;
        if (sum_mod == 0) break;
    }

    return (seq_t)k;
}
