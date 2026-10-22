/*
 * A354713  Number of solutions (n, D) for Pell equation n^2 - D*y^2 = 1 with
 *          fixed n.
 * https://oeis.org/A354713
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of the square root of the largest
 *            square dividing n^2-1 (equivalently, a(n) = d( floor( sqrt( (-1
 *            mod n^2) ) ) ) )
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Small structure to accumulate prime factors and their exponents. */
typedef struct {
    uint32_t prime;
    uint32_t exp;
} factor_t;

/* Add exponent e of prime p to the factor list. */
static void add_factor(factor_t *list, size_t *cnt, uint32_t p, uint32_t e)
{
    for (size_t i = 0; i < *cnt; ++i) {
        if (list[i].prime == p) {
            list[i].exp += e;
            return;
        }
    }
    list[*cnt].prime = p;
    list[*cnt].exp   = e;
    (*cnt)++;
}

/* Factorise val and merge its prime factors into list. */
static void factor_val(idx_t val, factor_t *list, size_t *cnt)
{
    if (val <= 1) return;
    idx_t t = val;
    uint32_t e = 0;

    /* Factor out powers of 2. */
    while ((t & 1u) == 0) {
        ++e;
        t >>= 1;
    }
    if (e) add_factor(list, cnt, 2, e);

    /* Factor odd primes. */
    for (idx_t i = 3; i * i <= t; i += 2) {
        e = 0;
        while (t % i == 0) {
            ++e;
            t /= i;
        }
        if (e) add_factor(list, cnt, (uint32_t)i, e);
    }

    /* Remaining prime if any. */
    if (t > 1) {
        add_factor(list, cnt, (uint32_t)t, 1);
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)1;       /* sequence is defined for n ≥ 2 */

    /* Collect prime exponents from both n-1 and n+1. */
    factor_t facs[64];
    size_t  fac_cnt = 0;

    factor_val(n - 1, facs, &fac_cnt);
    factor_val(n + 1, facs, &fac_cnt);

    /* Each prime's exponent in the square root is floor(total_exp / 2). */
    seq_t res = 1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint32_t total_exp = facs[i].exp;
        uint32_t half      = total_exp >> 1;          /* floor(total_exp/2) */
        res *= (seq_t)(half + 1);                     /* divisor count factor */
    }
    return res;
}
