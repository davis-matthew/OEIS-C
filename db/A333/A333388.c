/*
 * A333388  Number of solutions to x^y == -1 (mod n) where 0 <= x < n and 1 <=
 *          y <= n.
 * https://oeis.org/A333388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(x,y) : 0 ≤ x < n, 1 ≤ y ≤ n, x^y ≡ -1 (mod n)}|
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   _smallest_prime_factor, gcd, gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation:   factor n, use group‑theoretic characterisation of solutions */
/*        for each odd prime power p^e (φ = p^(e-1)(p-1)):         */
/*           solutions exist iff (φ / gcd(y,φ)) is even, in which case */
/*           there are gcd(y,φ) solutions modulo p^e.               */
/*        for 2^e:                                                    */
/*           e = 0 – nothing.                                         */
/*           e = 1 – exactly one solution for every y.              */
/*           e ≥ 2 – solutions exist iff y is odd, giving one solution. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* smallest prime factor utility from the utils library */
#include "../utils/gcd.h"          /* allows use of gcd for idx_t */
#include "../utils/gcd_int.h"      /* provides gcd for integer types */
#include "../utils/_smallest_prime_factor.h"

static idx_t
gcd_idx(idx_t a, idx_t b)
{
    while (b != 0u) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* factor n: record exponent of 2 and φ-values of odd prime powers */
    idx_t tmp = n;
    int e2 = 0;                        /* exponent of prime 2 */
    idx_t phi_vals[64];                /* φ for each odd prime power */
    int odd_cnt = 0;

    while (tmp > 1u) {
        idx_t p = _smallest_prime_factor(tmp);
        int exp = 0;
        while (tmp % p == 0u) {
            tmp /= p;
            ++exp;
        }

        if (p == 2u) {
            e2 = exp;
        } else {
            /* compute φ(p^exp) = p^(exp-1)*(p-1) */
            idx_t phi = 1;
            for (int i = 1; i < exp; ++i) {
                phi *= p;
            }
            phi *= (p - 1u);
            phi_vals[odd_cnt++] = phi;
        }
    }

    seq_t total = 0;
    /* iterate over all y from 1 to n */
    for (idx_t y = 1u; y <= n; ++y) {
        seq_t cnt = 1;                 /* solutions count for current y */

        /* 2-power component */
        if (e2 >= 2) {
            if ((y & 1u) == 0u) {
                continue;              /* even y gives no solutions */
            }
        }
        /* e2 == 1 or 0 gives one solution (or none) automatically */

        /* odd prime power components */
        for (int i = 0; i < odd_cnt; ++i) {
            idx_t phi = phi_vals[i];
            idx_t d = gcd_idx(y, phi);
            if ((phi / d) & 1u) {
                cnt = 0;                /* odd factor – no solution */
                break;
            }
            cnt *= (seq_t)d;           /* multiply by number of solutions */
        }

        if (cnt != 0) {
            total += cnt;
        }
    }
    return total;
}
