/*
 * A370164  The number of residues mod n that occur among the Markov numbers.
 * https://oeis.org/A370164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = u * C_s * D_t * ∏_{p≡3 (mod 4), p>3} (p−3)·p^(r−1) where
 *            n = 2^s · 3^t · u · ∏ p_i^r_i, u contains only primes ≡1 (mod
 *            4), and p_i ≡3 (mod 4), p_i>3
 * Range:     u8 n <= 256, u16 n <= 9983, u32 n <= 9983, u64 n <= 9983,
 *            u128 n <= 9983, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* C_s = 2 (s=1), 2 (s=2), 3 (s=3), 5 (s=4), 9·2^(s−5) for s≥5.   */
/* D_t = 1 (t=0), 2·3^(t−1) for t≥1.                               */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow_uint.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, defensive */
    if (n == 1) return 1;               /* a(1) = 1 */

    /* Factorise n */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Extract s, t, compute u, and process primes ≡3 (mod 4) >3 */
    idx_t    s = 0;     /* exponent of 2 */
    idx_t    t = 0;     /* exponent of 3 */
    seq_t    u = 1;     /* product of p^e, p ≡1 (mod 4) */
    seq_t    pt_fact = 1; /* product over p≡3 (mod 4) >3 of (p−3)·p^(e−1) */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        idx_t    e = (idx_t)exps[i];

        if (p == 2) {
            s = e;
            continue;
        }
        if (p == 3) {
            t = e;
            continue;
        }
        if ((p & 3ULL) == 1ULL) {          /* p ≡ 1 (mod 4) */
            u *= pow_uint(p, e);
        } else {                           /* p ≡ 3 (mod 4), p>3 */
            seq_t factor = pow_uint(p, e > 1 ? e - 1 : 0); /* p^(e-1) */
            factor *= (seq_t)(p - 3);
            pt_fact *= factor;
        }
    }

    /* Compute C_s */
    seq_t C_s;
    if (s == 0) {
        C_s = 1;
    } else if (s == 1) {
        C_s = 2;
    } else if (s == 2) {
        C_s = 2;
    } else if (s == 3) {
        C_s = 3;
    } else if (s == 4) {
        C_s = 5;
    } else { /* s >= 5 */
        seq_t odd  = pow_uint(2, s - 2);   /* 2^(s-2) */
        seq_t even = pow_uint(2, s - 5);   /* 2^(s-5) */
        C_s = odd + even;                  /* 9·2^(s-5) */
    }

    /* Compute D_t */
    seq_t D_t;
    if (t == 0) {
        D_t = 1;
    } else {
        seq_t pow3 = pow_uint(3, t - 1); /* 3^(t-1) */
        D_t = pow3 * 2ULL;
    }

    /* Assemble final result */
    seq_t result = 1;
    result *= u;
    result *= C_s;
    result *= D_t;
    result *= pt_fact;

    return result;
}
