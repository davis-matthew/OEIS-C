/*
 * A331142  Number of different coincidence site lattices of index n in
 *          lattice A_4.
 * https://oeis.org/A331142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e∥n} a(p) · p^{2(e‑1)}, where a(p) = 6 if p = 5,
 *            a(p) = p^2 + 1 if p ≡ ±2 (mod 5), a(p) = (p + 1)^2 if p ≡ ±1
 *            (mod 5); a(1) = 1. The function is multiplicative and factors n
 *            by its prime powers. It uses seq_pow for power computation
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Prime factorization helper  */
#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;                     /* a(1) = 1, and a(0) is not used */

    seq_t result = 1;
    idx_t m = n;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p != 0)
            continue;

        idx_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }

        /* compute a(p) */
        seq_t a_p;
        if (p == 5) {
            a_p = (seq_t)6;
        } else if (p % 5 == 1 || p % 5 == 4) {   /* p ≡ ±1 (mod 5) */
            seq_t pp1 = (seq_t)(p + 1);
            a_p = pp1 * pp1;
        } else {                                 /* p ≡ ±2 (mod 5) */
            seq_t pp = (seq_t)p * (seq_t)p;
            a_p = pp + 1;
        }

        result *= a_p;

        if (e > 1) {
            seq_t p2 = (seq_t)p * (seq_t)p;              /* p² */
            seq_t power = seq_pow(p2, (idx_t)(e - 1));   /* (p²)^(e‑1) */
            result *= power;
        }
    }

    /* if m > 1, it is a remaining prime > sqrt(original n) */
    if (m > 1) {
        idx_t p = m;
        seq_t a_p;
        if (p == 5) {
            a_p = (seq_t)6;
        } else if (p % 5 == 1 || p % 5 == 4) {   /* p ≡ ±1 (mod 5) */
            seq_t pp1 = (seq_t)(p + 1);
            a_p = pp1 * pp1;
        } else {                                 /* p ≡ ±2 (mod 5) */
            seq_t pp = (seq_t)p * (seq_t)p;
            a_p = pp + 1;
        }
        result *= a_p;
    }

    return result;
}
