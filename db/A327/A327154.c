/*
 * A327154  a(n) = Product_{d|n, d>1} A008578(1+A286561(sigma(n),d)), where
 *          A286561(n,d) gives the highest exponent of d dividing n.
 * https://oeis.org/A327154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d>1} A008578(1+A286561(sigma(n), d))
 * Range:     u8 n <= 83, u16 n <= 119, u32 n <= 3359, u64 n <= 9933,
 *            u128 n <= 9933, big any n
 * Helpers:   factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/sigma_of.h"
#include "../utils/factor_u64.h"

static const seq_t A008578_values[] = {
    0,      /* unused index 0 */
    1,      /* A008578(1) = 1 */
    2,      /* A008578(2) = 2 */
    3,      /* A008578(3) = 3 */
    5,      /* A008578(4) = 5 */
    7,      /* A008578(5) = 7 */
    11,     /* A008578(6) = 11 */
    13,     /* A008578(7) = 13 */
    17,     /* A008578(8) = 17 */
    19,     /* A008578(9) = 19 */
    23,     /* A008578(10) = 23 */
    29,     /* A008578(11) = 29 */
    31,     /* A008578(12) = 31 */
    37,     /* A008578(13) = 37 */
    41,     /* A008578(14) = 41 */
    43,     /* A008578(15) = 43 */
    47,     /* A008578(16) = 47 */
    53,     /* A008578(17) = 53 */
    59,     /* A008578(18) = 59 */
    61,     /* A008578(19) = 61 */
    67,     /* A008578(20) = 67 */
    71,     /* A008578(21) = 71 */
    73,     /* A008578(22) = 73 */
    79,     /* A008578(23) = 79 */
    83,     /* A008578(24) = 83 */
    89      /* A008578(25) = 89 */
};

static void enum_divisors(const uint64_t *pr, const uint64_t *ex,
                          size_t cnt, size_t pos, uint64_t cur,
                          seq_t sigma, seq_t *product)
{
    if (pos == cnt) {
        if (cur > 1) {
            /* compute v_cur(sigma) */
            idx_t e = 0;
            seq_t temp = sigma;
            seq_t d_seq = (seq_t)cur;
            while (temp % d_seq == 0) {
                temp /= d_seq;
                ++e;
            }
            /* argument to A008578 is 1 + e */
            size_t index = (size_t)(e + 1); /* e is small, < max size */
            seq_t factor = A008578_values[index];
            *product *= factor;
        }
        return;
    }
    uint64_t mult = 1;
    for (uint64_t expo = 0; expo <= ex[pos]; ++expo) {
        enum_divisors(pr, ex, cnt, pos + 1, cur * mult, sigma, product);
        mult *= pr[pos];
    }
}

seq_t a(idx_t n)
{
    if ((idx_t)1 == n) return 1;

    seq_t sigma = sigma_of((uint64_t)n);

    /* factor n: we may need up to 64 primes */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t product = 1;
    enum_divisors(primes, exps, cnt, 0, 1, sigma, &product);
    return product;
}
