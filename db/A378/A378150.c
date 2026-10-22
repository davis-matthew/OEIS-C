/*
 * A378150  a(n) is the number of distinct integer-sided isosceles trapezoids
 *          with exactly one pair of parallel sides and area n.
 * https://oeis.org/A378150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating all integer heights h dividing n and for each h
   counting integer differences x such that x² + h² is a perfect square,
   and that x < n/h.  Each such (h, x) corresponds to one distinct
   integer‑sided isosceles trapezoid with area n. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Recursively collect all divisors of the number represented by primes[]
   with exponents exp[], up to the limit 'max'.  The divisor list is
   appended to 'divs', and 'cnt' counts them. */
static void
gen_divs_rec(uint64_t *primes, uint64_t *exp, size_t pos, uint64_t cur, uint64_t max,
             uint64_t *divs, size_t *cnt)
{
    if (pos == 0) {
        if (cur <= max)
            divs[(*cnt)++] = cur;
        return;
    }
    uint64_t p  = primes[pos-1];
    uint64_t val = cur;
    for (uint64_t k=0; k<=exp[pos-1]; ++k) {
        gen_divs_rec(primes, exp, pos-1, val, max, divs, cnt);
        val *= p;
    }
}

/* Count trapezoids for a fixed height h and area n. */
static seq_t
count_for_h(idx_t h, idx_t n)
{
    uint64_t h2 = (uint64_t)h * (uint64_t)h;        /* h²  <= 10⁸, fits 64‑bit */
    uint64_t t  = n / h;                            /* q + x = n/h   */

    /* Factor h to get primes and exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(h, primes, exps, &pcnt);

    /* Exponents for h² are double those of h. */
    uint64_t exps2[64];
    for (size_t i = 0; i < pcnt; ++i)
        exps2[i] = exps[i] * 2;

    /* Generate all divisors a of h² with a ≤ h.  These are the
       possible values of a = d – x. */
    uint64_t divs[8192];
    size_t   dcnt = 0;
    gen_divs_rec(primes, exps2, pcnt, 1, (uint64_t)h, divs, &dcnt);

    seq_t cnt = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t a = divs[i];
        uint64_t b = h2 / a;            /* a * b = h² ,  b ≥ a  */

        /* a and b must have the same parity so that (b–a)/2 is integer. */
        if ( (a & 1) != (b & 1) )
            continue;

        uint64_t diff = b - a;          /* always even because parities match */
        uint64_t x    = diff / 2;        /* difference side of the right triangle */

        if (x == 0)
            continue;                   /* would give equal bases, not a trapezoid */

        if (x < t)
            cnt++;                      /* q = t – x > 0  */
    }
    return cnt;
}

/* Main sequence entry function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t total = 0;

    /* Enumerate all height divisors h of n. */
    for (idx_t h = 1; h <= n / h; ++h) {
        if (n % h == 0) {
            total += count_for_h(h, n);
            if (h != n / h)           /* avoid double counting the square root divisor */
                total += count_for_h(n / h, n);
        }
    }
    return total;
}
