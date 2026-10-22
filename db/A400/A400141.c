/*
 * A400141  The number of divisors d of n such that gcid(d, n/d) is an
 *          exponentially odd number (A268335), where gcid is the greatest
 *          common infinitary divisor.
 * https://oeis.org/A400141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A400141: number of divisors d of n such that gcid(d,n/d) is exponentially odd
 *  The sequence is multiplicative with
 *      a(p^e) = f(e)
 *  where f(e) counts the integers k, 0 ≤ k ≤ e, for which
 *      (k & (e - k)) == 0   or   (k & (e - k)) is odd.
 *  This follows directly from the definition of the greatest common infinitary
 *  divisor.
 *
 *  The implementation precomputes f(e) up to the maximum exponent that can
 *  occur for n up to 10^64 (but for n ≤ 10 000 the table is tiny).
 *  The function factors n using the utility `factor_u64` and then multiplies
 *  the corresponding f(e) values.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static seq_t *f_values = NULL;
static size_t f_max = 0;

/* Compute f(e) for 0 ≤ e ≤ max_e and store in f_values. */
static void compute_f_table(size_t max_e)
{
    f_values = (seq_t *)calloc(max_e + 1, sizeof(seq_t));
    f_values[0] = (seq_t)1;   /* unused but harmless */

    for (size_t e = 1; e <= max_e; ++e) {
        size_t cnt = 0;
        for (size_t k = 0; k <= e; ++k) {
            size_t t = k & (e - k);
            if (t == 0 || (t & 1U) != 0)
                ++cnt;
        }
        f_values[e] = (seq_t)cnt;
    }
    f_max = max_e;
}

seq_t a(idx_t n)
{
    /* lazy initialization of f_values for maximum exponent 64,
     * sufficient for any 64‑bit n and more than enough for 10 000. */
    if (f_values == NULL)
        compute_f_table(64);

    if (n <= 1)
        return (seq_t)1;          /* a(1) = 1 */

    uint64_t N = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt_primes;

    factor_u64(N, primes, exps, &cnt_primes);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt_primes; ++i) {
        size_t e = (size_t)exps[i];
        if (e > f_max)
            compute_f_table(e);   /* extend table if needed */

        result *= f_values[e];
    }
    return result;
}
