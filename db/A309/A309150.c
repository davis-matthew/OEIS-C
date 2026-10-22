/*
 * A309150  Number of solutions of the Diophantine equation 1/n + 1/x = 1/y +
 *          1/z, where n >= 1, x > n, y > n and z > y.
 * https://oeis.org/A309150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (x,y,z) of positive integers satisfying
 *            1/n + 1/x = 1/y + 1/z with n ≥ 1, x > n, y > n and z > y. The
 *            value a(n) is the count of distinct solutions
 * Range:     u8 n <= 23, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   build_divs_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_divs_rec.h"

/* prime sieve up to 15000 (enough for sqrt(2·max(n)·max(y)) ≤ 15000 when n ≤ 10000) */
static uint32_t sprimes[2000];
static size_t sprime_cnt = 0;

static void init_primes(void)
{
    if (sprime_cnt) return;
    bool composite[15001] = {false};
    for (int i = 2; i * i <= 15000; ++i)
        if (!composite[i])
            for (int j = i * i; j <= 15000; j += i)
                composite[j] = true;
    for (int i = 2; i <= 15000; ++i)
        if (!composite[i]) sprimes[sprime_cnt++] = (uint32_t)i;
}

/* factorise an unsigned 64‑bit integer n into primes and exponents.
   The contents are stored in arrays fp[] and fe[], and the number of
   distinct prime factors is written to cnt. */
static void factor_u64(uint64_t n, uint32_t *fp, int *fe, size_t *cnt)
{
    *cnt = 0;
    for (size_t i = 0; i < sprime_cnt && (uint64_t)sprimes[i] * sprimes[i] <= n; ++i) {
        uint32_t p = sprimes[i];
        if (n % p == 0) {
            int c = 0;
            while (n % p == 0) { n /= p; ++c; }
            fp[(*cnt)++] = p;
            fe[(*cnt)-1] = c;
        }
    }
    if (n > 1) {          /* remaining prime factor */
        fp[(*cnt)++] = (uint32_t)n;
        fe[(*cnt)-1] = 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1, but never requested for 0 */
    init_primes();

    uint64_t nn = (uint64_t)n;
    seq_t result = 0;

    /* iterate y ; y must satisfy n < y ≤ 2n-1   (see derivation) */
    for (uint64_t y = nn + 1; y <= 2 * nn - 1; ++y) {
        uint64_t A = y - nn;             /* positive */
        uint64_t B = nn * y;             /* B ≤ 2·10⁸ for n ≤ 10⁴ */

        /* factorise B */
        uint32_t pf[20];
        int     pf_exp[20];
        size_t  pf_cnt;
        factor_u64(B, pf, pf_exp, &pf_cnt);

        /* generate all divisors of B² */
        static uint64_t divs[200000];
        size_t div_cnt = 0;
        build_divs_rec(pf, pf_exp, pf_cnt, 0, 1, divs, &div_cnt);

        /* test each divisor d = B - z·A (must satisfy 0 < d < B) */
        for (size_t i = 0; i < div_cnt; ++i) {
            uint64_t d = divs[i];
            if (d == 0 || d >= B) continue;           /* enforce B - z·A > 0 */

            uint64_t diff = B - d;
            if (diff % A != 0) continue;               /* z must be integer */

            uint64_t z = diff / A;
            if (z <= y) continue;                     /* z > y */

            __uint128_t num = (__uint128_t)z * B;
            if ((uint64_t)(num % d) != 0) continue;    /* ensure integer x */

            uint64_t x = (uint64_t)(num / d);
            if (x <= nn) continue;                    /* x > n */

            /* the triple (x,y,z) is a valid solution */
            ++result;
        }
    }
    return result;
}
