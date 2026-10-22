/*
 * A336915  a(n) is the exponent of the least power of 2 that when multiplied
 *          by n, makes the product nondeficient, or -1 if n itself is a power
 *          of 2.
 * https://oeis.org/A336915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 66, i16 n <= 7952, i32 n <= 7952, i64 n <= 7952,
 *            i128 n <= 7952, big any n
 * Helpers:   pow2, sigma_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) function for OEIS sequence A336915
 *
 *  a(n) is the smallest exponent k such that σ(2^k · n) ≥ 2·(2^k · n),
 *  where σ denotes the sum-of-divisors function.  If n is a power of 2,
 *  a(n) = –1.
 *
 *  We compute σ(·) multiplicatively:
 *      σ(2^e)   = 2^(e+1) – 1
 *      σ(m)     for odd m is computed by trial‑division.
 *
 *  For n = 2^e · m (m odd) the inequality
 *      σ(2^{e+k}) · σ(m) ≥ 2^{e+k+1} · m
 *  reduces to
 *      2^{e+k+1} · (σ(m) – m) ≥ σ(m).
 *  So we find the smallest k≥0 with this property by trying successive
 *  powers of 2.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */
#include "../utils/sigma_odd.h"

seq_t a(idx_t n)
{
    /* n is a power of two?  → –1 */
    if (((n & (n - 1)) == 0) && n > 0) {
        return (seq_t)-1;
    }

    /* factor out powers of 2:  n = 2^e · m   (m odd, m>1) */
    idx_t e = 0;
    idx_t tmp = n;
    while ((tmp & 1) == 0) {
        tmp >>= 1;
        e++;
    }
    idx_t m = tmp;                      /* odd part, m > 1 */

    /* σ(m) and A = σ(m) – m */
    seq_t sigma_m = sigma_odd(m);
    seq_t A = sigma_m - (seq_t)m;        /* guaranteed > 0 for m>1 */

    /* Find smallest k≥0 such that 2^(e+k+1)·A ≥ σ(m) */
    idx_t k   = 0;
    idx_t exp = e + 1;                    /* exponent in 2^exp */
    while (true) {
        seq_t power = pow2(exp);          /* 2^exp as seq_t */
        if (power * A >= sigma_m) {
            break;
        }
        k++;
        exp++;
    }
    return (seq_t)k;
}
