/*
 * A266288  Expansion of a(q)^2 * (c(q)/3)^3 in powers of q where a(), c() are
 *          cubic AGM theta functions.
 * https://oeis.org/A266288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A266288 – Expansion of a(q)^2 * (c(q)/3)^3.
   Formula: a is multiplicative with
   a(p^e) = ((p^4)^(e+1) - s^(e+1)) / (p^4 - s),
   where s = 0 for p = 3,
          s = 1 for p ≡ 1 (mod 3),
          s = -1 for p ≡ 2 (mod 3).
   The function computes a(n) for any n ≥ 1 without using external
   libraries beyond the standard C headers.
*/

#include <stdint.h>
#include <stddef.h>

/* Fast power: base^exp for unsigned integer types. */
static seq_t pow_seq(seq_t base, uint64_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset starts at 1, 0 is unused */

    /* factor n by trial division */
    uint64_t m = n;
    uint64_t primes[64];
    uint32_t exps[64];
    size_t num = 0;

    for (uint64_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            uint32_t e = 0;
            while (m % d == 0) {
                m /= d;
                ++e;
            }
            primes[num] = d;
            exps[num]   = e;
            ++num;
        }
    }
    if (m > 1) {                     /* remaining prime factor */
        primes[num] = m;
        exps[num]   = 1;
        ++num;
    }

    seq_t result = 1;

    for (size_t i = 0; i < num; ++i) {
        uint64_t p = primes[i];
        uint32_t e = exps[i];

        seq_t term = 1;

        if (p == 3) {                /* s = 0 case: term = p^(4*e) */
            term = pow_seq((seq_t)p, (uint64_t)4 * e);
        } else if (p % 3 == 1) {     /* s = 1 case: geometric series */
            seq_t p4 = pow_seq((seq_t)p, 4);
            term = 1;                /* start with k = 0 term */
            seq_t curr = 1;
            for (uint32_t k = 1; k <= e; ++k) {
                curr *= p4;
                term += curr;
            }
        } else {                     /* p % 3 == 2, s = -1 case */
            seq_t p4 = pow_seq((seq_t)p, 4);
            /* Compute S = 1 + Σ_{k=1}^e (-1)^k p4^k */
            __int128 S = 1;
            __int128 curr = 1;
            int sign = -1;           /* first added term has sign -1 */
            for (uint32_t k = 1; k <= e; ++k) {
                curr *= ( __int128 )p4;
                if (sign > 0) S += curr;
                else          S -= curr;
                sign = -sign;
            }
            if (e & 1) S = -S;       /* multiply by (-1)^e */
            term = (seq_t)S;         /* S is non‑negative here */
        }

        result *= term;
    }

    return result;
}
