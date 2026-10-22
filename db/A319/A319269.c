/*
 * A319269  Number of uniform factorizations of n into factors > 1, where a
 *          factorization is uniform if all factors appear with the same
 *          multiplicity.
 * https://oeis.org/A319269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|gcd of prime exponents of n} A045778( n^(1/d) )
 *            (A045778 = number of unordered factorizations of an integer into
 *            distinct factors > 1)
 * Range:     u8 n <= 63, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   factor_u64, floor_sqrt, int_gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Utility functions available in the project */
#include "../utils/factor_u64.h"   /* factor_u64 */
#include "../utils/floor_sqrt.h"  /* floor_sqrt */
#include "../utils/pow_uint.h"    /* pow_uint */
#include "../utils/int_gcd.h"

/* -------------------------------------------------------------------- */
/*  Helper: integer gcd of two positive ints                            */

/*  Helper: compute m = n^(1/r) by dividing all prime exponents       */
static uint64_t root_of_n(uint64_t n, const uint64_t *pr, const uint64_t *ex,
                          size_t pf_cnt, uint64_t r)
{
    uint64_t m = 1;
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t pow_exp = ex[i] / r;           /* integer division */
        seq_t pwr = pow_uint(pr[i], pow_exp);   /* seq_t result fits in uint64_t  */
        m *= (uint64_t)pwr;                     /* safe: product <= n           */
    }
    return m;
}

/*  Recursive count of unordered factorizations of m into distinct >1 */
static seq_t count_distinct_rec(uint64_t rem, uint64_t min_f)
{
    /* Base: using rem itself as last factor */
    seq_t cnt = 1;

    uint64_t limit = (uint64_t)floor_sqrt((seq_t)rem);
    for (uint64_t d = min_f; d <= limit; ++d) {
        if (rem % d == 0) {
            uint64_t rest = rem / d;
            if (rest >= d + 1)           /* enforce increasing order */
                cnt += count_distinct_rec(rest, d + 1);
        }
    }
    return cnt;
}

static seq_t count_distinct(uint64_t m)
{
    if (m <= 1) return 0;          /* no factorizations for 1 */
    return count_distinct_rec(m, 2);
}

/* -------------------------------------------------------------------- */
/*  Main sequence function                                              */
seq_t a(idx_t n)
{
    /* Handle offset: a(1)=1 */
    if (n == 1) return (seq_t)1;

    /* Factorization of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pf_cnt);

    /* GCD of the prime exponents */
    int g = (int)exps[0];
    for (size_t i = 1; i < pf_cnt; ++i)
        g = int_gcd(g, (int)exps[i]);

    /* Sum over all divisors d of g */
    seq_t ans = 0;
    for (int r = 1; r <= g; ++r) {
        if (g % r != 0) continue;

        uint64_t m = root_of_n((uint64_t)n, primes, exps, pf_cnt, (uint64_t)r);
        ans += count_distinct(m);
    }
    return ans;
}
