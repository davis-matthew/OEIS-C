/*
 * A320410  Continued fraction of a constant t with partial denominators
 *          {a(n), n>=0} such that the continued fraction of 4*t yields
 *          partial denominators {6*a(n), n>=0}.
 * https://oeis.org/A320410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (n mod 4 == 3 ? 4 : 0) * + Σ_{k=3}^{⌊log₂(n+1)⌋}
 *            115·24^(k‑3) if (n+1) mod 2^k == 0 * * The sequence is computed
 *            directly from its generating function: * S(x) = 1/(1‑x) +
 *            4x³/(1‑x⁴) + Σ_{k≥3} 115·24^(k‑3)·x^(2^k‑1)/(1‑x^(2^k)). *
 *            Consequently, a(n) is 1 everywhere, plus 4 at indices ≡...
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 510, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 1;                     /* base term from 1/(1‑x) */

    /* Add the 4 from 4x³/(1‑x⁴) for indices 3,7,11,… (i.e. n≡3 mod 4) */
    if ((n & 3) == 3)
        res += 4;

    /* Process the infinite sum part.
       For each k ≥ 3, if (n+1) is divisible by 2^k,
       we add 115·24^(k‑3). */
    idx_t m      = n + 1;              /* n+1 appears in the divisibility test */
    seq_t pow24  = 1;                  /* 24^(k‑3) for current k */

    for (idx_t k = 3; ; ++k) {
        idx_t step = 1ULL << k;        /* 2^k */
        if (step > m)                 /* no more factors can divide n+1 */
            break;
        if ((m % step) == 0)
            res += (seq_t)115 * pow24;
        pow24 *= 24;                  /* prepare 24^(k‑2) for next iteration */
    }

    return res;
}
