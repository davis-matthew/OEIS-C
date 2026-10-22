/*
 * A319191  Coefficient of p(y) / A056239(n)! in Product_{i >= 1} (1 + x_i),
 *          where p is power-sum symmetric functions and y is the integer
 *          partition with Heinz number n.
 * https://oeis.org/A319191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(∑ x_i*y_i – ∑ y_i) * (∑ x_i*y_i)! / (∏ x_i^y_i · ∏
 *            y_i!)
 * Range:     i8 n <= 12, i16 n <= 18, i32 n <= 40, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   factor_u64, factorial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where n = ∏ prime(x_i)^y_i is the prime factorisation of n,
   prime(1)=2, prime(2)=3, etc. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*--- utility headers -----------------------------------------------------*/
#include "../utils/factor_u64.h"   /* factor_u64()                            */
#include "../utils/pow_seq.h"      /* pow_seq()                              */
#include "../utils/factorial.h"    /* factorial()                            */

/*--- Prime sieve for finding prime indices -----------------------------*/
#define PRIME_LIMIT 2000000ULL    /* good enough for n ≤ 100 000 */

static uint64_t *prime_list = NULL;   /* array of primes */
static size_t   prime_cnt   = 0;
static bool     primes_ready = false;

/* Build the sieve once */
static void init_primes(void)
{
    if (primes_ready) return;

    size_t limit = PRIME_LIMIT + 1;          /* include limit itself */
    bool *is_comp = (bool *)calloc(limit, sizeof(bool));

    for (size_t i = 2; i * i < limit; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j < limit; j += i)
                is_comp[j] = true;
        }
    }

    /* count primes */
    for (size_t i = 2; i < limit; ++i)
        if (!is_comp[i])
            ++prime_cnt;

    /* store primes */
    prime_list = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    size_t pos = 0;
    for (size_t i = 2; i < limit; ++i)
        if (!is_comp[i])
            prime_list[pos++] = i;

    free(is_comp);
    primes_ready = true;
}

/* Return 1‑based index of a prime, 0 if p is not in the table      */
static inline uint64_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < prime_cnt && prime_list[lo] == p) ? (uint64_t)(lo + 1) : 0;
}

/*--- Sequence function --------------------------------------------------*/
seq_t a(idx_t n)
{
    /* behaviour for indices below the offset is irrelevant */
    if (n == 0) return 0;

    init_primes();

    /* Factorise n */
    uint64_t fac_pr[64];
    uint64_t fac_ex[64];
    size_t fac_cnt = 0;
    factor_u64((uint64_t)n, fac_pr, fac_ex, &fac_cnt);

    /* Compute sum(x_i*y_i) and sum(y_i) */
    idx_t sum_xy = 0;
    idx_t sum_y  = 0;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p  = fac_pr[i];
        uint64_t e  = fac_ex[i];
        uint64_t idxp = prime_index(p);           /* 1‑based prime index */
        sum_xy += (idx_t)(idxp * e);
        sum_y  += (idx_t)e;
    }

    /* Sign factor */
    int sign = ((sum_xy - sum_y) & 1) ? -1 : 1;

    /* Numerator = factorial(sum_xy) */
    seq_t numerator = factorial(sum_xy);

    /* Denominator = ∏ x_i^y_i · ∏ y_i! */
    seq_t denom = 1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p  = fac_pr[i];
        uint64_t e  = fac_ex[i];
        uint64_t idxp = prime_index(p);
        denom *= pow_seq((seq_t)idxp, (int)e);      /* x_i^y_i   */
        denom *= factorial((idx_t)e);               /* y_i!      */
    }

    /* Exact division followed by sign */
    seq_t res = numerator / denom;
    return res * sign;
}
