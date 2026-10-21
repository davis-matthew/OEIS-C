/*
 * A261072  Divisors of 1234567890987654321.
 * https://oeis.org/A261072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 22, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   factor_u64, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th divisor of 1234567890987654321
   in ascending order (a(1)=1, a(2)=3, …).  The divisors are
   generated once by factoring the number and enumerating all
   combinations of its prime powers. */

/* required utilities */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"  /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/seq_cmp.h"     /* int seq_cmp(const void *, const void *); */

/* static storage for the divisors */
static seq_t divisors[64];
static size_t divcnt = 0;
static bool initialized = false;

/* recursive enumeration of all divisors given prime factor arrays */
static void gen_divisors(size_t idx,
                         const uint64_t *primes,
                         const uint64_t *exps,
                         size_t num_factors,
                         uint64_t cur)
{
    if (idx == num_factors) {
        /* store the divisor, casting safely to seq_t */
        divisors[divcnt++] = (seq_t)cur;
        return;
    }
    uint64_t power = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        gen_divisors(idx + 1, primes, exps, num_factors,
                     cur * power);
        power *= primes[idx];
    }
}

/* initialize the divisor table once */
static void init_divisors(void)
{
    const uint64_t target = 1234567890987654321ULL;

    /* maximum number of distinct prime factors is small (≤16) */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t num_factors;

    /* use the provided factor_u64 utility */
    factor_u64(target, primes, exps, &num_factors);

    /* enumerate all divisors */
    gen_divisors(0, primes, exps, num_factors, 1ULL);

    /* sort the array */
    qsort(divisors, divcnt, sizeof(seq_t), seq_cmp);

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized)
        init_divisors();

    if (n == 0)
        return (seq_t)0;
    idx_t idx = n - 1;
    if (idx < divcnt)
        return divisors[idx];
    return (seq_t)0;
}
