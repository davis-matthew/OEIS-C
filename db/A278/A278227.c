/*
 * A278227  Least number with the prime signature of prime(n)-1.
 * https://oeis.org/A278227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046523(A000040(n)-1) (least number with the same prime
 *            signature as prime(n)-1)
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, get_prime, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* Comparator for descending order of exponents */
static int cmp_desc(const void *a, const void *b)
{
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    if (*x < *y) return 1;
    if (*x > *y) return -1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n=0, safe fallback */

    /* Prime(n) */
    uint32_t p = get_prime(n);
    uint64_t m = (uint64_t)p - 1ULL;

    /* The only case where prime(n)-1 == 0 or 1 */
    if (m <= 1ULL) return (seq_t)1;

    /* Allocate temporary arrays for factorization (large enough for any 64‑bit number) */
    const size_t MAX_FACTORS = 64;
    uint64_t *pr = (uint64_t *)malloc(sizeof(uint64_t) * MAX_FACTORS);
    uint64_t *ex = (uint64_t *)malloc(sizeof(uint64_t) * MAX_FACTORS);
    if (!pr || !ex) { free(pr); free(ex); return (seq_t)0; }

    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);

    /* Copy exponents to a separate array for sorting */
    uint64_t *exp = (uint64_t *)malloc(sizeof(uint64_t) * cnt);
    if (!exp) { free(pr); free(ex); return (seq_t)0; }
    for (size_t i = 0; i < cnt; ++i) exp[i] = ex[i];

    /* Sort exponents descending: largest exponent gets smallest prime (2,3,5,…) */
    qsort(exp, cnt, sizeof(uint64_t), cmp_desc);

    /* Build the minimal number with the same prime signature */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t base   = (seq_t)get_prime((idx_t)(i + 1));
        seq_t power  = seq_pow(base, (idx_t)exp[i]);
        result       = seq_mul(result, power);
    }

    free(pr);
    free(ex);
    free(exp);
    return result;
}
