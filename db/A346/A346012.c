/*
 * A346012  a(n) is the numerator of A346009(n)/A346010(n) - A001221(n)/2.
 * https://oeis.org/A346012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of f(n) where f(n) = Σ_{p^e‖n} (e‑1) /
 *            (2*(e+1)) (terms with e ≤ 1 contribute 0). The numerator is
 *            returned after the fraction has been reduced
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* Prime factorisation of n (uint64_t is enough for idx_t). */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t num = 0;   /* accumulated numerator   */
    seq_t den = 1;   /* accumulated denominator */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = ex[i];
        if (e <= 1) continue;                    /* term is zero */

        seq_t term_num = (seq_t)(e - 1);          /* e‑1 */
        seq_t term_den = (seq_t)(2 * (e + 1));    /* 2·(e+1) */

        if (num == 0) {                          /* first non‑zero term */
            num = term_num;
            den = term_den;
            continue;
        }
        /* Add term_num/term_den to the running fraction num/den. */
        seq_t new_num = num * term_den + term_num * den;
        seq_t new_den = den * term_den;
        seq_t g = seq_gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    /* Reduce the final fraction (necessary if only one term was added). */
    if (num != 0) {
        seq_t g = seq_gcd(num, den);
        num /= g;
        /* den is no longer needed; only the numerator is returned. */
    }

    return num;   /* numerator of the reduced fraction f(n) */
}
