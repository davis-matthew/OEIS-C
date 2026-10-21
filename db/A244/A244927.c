/*
 * A244927  Least positive number k such that k*n written in base 10 is either
 *          a repunit or of the form 1111....0000.
 * https://oeis.org/A244927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least positive k such that k*n written in base 10 is
 *            either a repunit (111…1) or a repunit followed by zeros. The
 *            algorithm factorizes n = 2^a 5^b m with gcd(m,10)=1, computes
 *            the minimal repunit length l such that m divides R_l, then k =
 *            (R_l * 10^t) / n where t = max(a,b)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   pow10, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/powmod64.h"

/* multiplicative order of 10 modulo 9*m (m coprime to 10) */
static uint64_t multiplicative_order(uint64_t m)
{
    uint64_t mod = 9 * m;
    /* compute φ(mod) */
    uint64_t phi = mod;
    uint64_t tmp = mod;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            phi = phi / p * (p - 1);
        }
    }
    if (tmp > 1) {
        phi = phi / tmp * (tmp - 1);
    }

    /* find multiplicative order of 10 modulo mod */
    uint64_t ord = phi;
    tmp = phi;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            while (ord % p == 0 && powmod64(10, ord / p, mod) == 1) {
                ord /= p;
            }
        }
    }
    if (tmp > 1) {
        uint64_t p = tmp;
        while (ord % p == 0 && powmod64(10, ord / p, mod) == 1) {
            ord /= p;
        }
    }
    return ord;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, for safety */
    uint64_t orig = (uint64_t)n;

    /* factor out powers of 2 and 5 */
    uint64_t m = orig;
    uint64_t aexp = 0, bexp = 0;
    while ((m & 1ULL) == 0) { m >>= 1; ++aexp; }
    while (m % 5ULL == 0) { m /= 5ULL; ++bexp; }

    uint64_t t = (aexp > bexp) ? aexp : bexp;   /* minimal zeros needed */

    if (m == 1) {   /* n has only 2 and 5 factors */
        seq_t res = pow10(t);
        return res / (seq_t)orig;   /* exact division */
    }

    uint64_t l = multiplicative_order(m);   /* repunit length */

    seq_t repunit = (pow10(l) - 1) / 9;  /* R_l = (10^l-1)/9 */
    seq_t res = repunit * pow10(t);      /* M = R_l * 10^t */
    res /= (seq_t)orig;                  /* k = M / n */
    return res;
}
