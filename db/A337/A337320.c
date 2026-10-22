/*
 * A337320  Number of ordered pairs of divisors of n, (d1,d2), such that d1 is
 *          prime and d1 < d2.
 * https://oeis.org/A337320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = number of ordered pairs of divisors (d1,d2) of n with d1
 *            prime and d1 < d2
 * Range:     u8 n <= 4699, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, factor_u64, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Helper prototypes – provided by the build system */
#include "../utils/factor_u64.h"          /* void factor_u64(uint64_t n, uint64_t* pr, uint64_t* ex, size_t* cnt); */
#include "../utils/is_prime_small.h"     /* bool is_prime_small(seq_t n); */
#include "../utils/divisor_count_u64.h"  /* uint64_t divisor_count_u64(uint64_t n); */

/* Recursive generation of all divisors from the prime factorisation. */
static void
gen_divs_rec(size_t idx, uint64_t curr, const uint64_t *pr,
             const uint64_t *ex, size_t cnt, uint64_t *divs,
             size_t *pos)
{
    if (idx == cnt) {
        divs[*pos] = curr;
        (*pos)++;
        return;
    }

    uint64_t p   = pr[idx];
    uint64_t val = curr;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        gen_divs_rec(idx + 1, val, pr, ex, cnt, divs, pos);
        val *= p;
    }
}

/* Comparison function for qsort – sorts uint64_t values ascending. */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return -1;
    if (va > vb) return  1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 2) {
        return (seq_t)0;
    }

    /* --- 1. Factor the number ------------------------------------------ */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   pcnt  = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* --- 2. Allocate space for all divisors ----------------------------- */
    uint64_t dcnt_est = divisor_count_u64((uint64_t)n);
    uint64_t *divs    = (uint64_t *)malloc(dcnt_est * sizeof(uint64_t));
    if (!divs) {
        /* allocation failure – return 0 as a safe default. */
        return (seq_t)0;
    }

    /* --- 3. Generate all divisors --------------------------------------- */
    size_t dcnt = 0;
    gen_divs_rec(0, 1ULL, primes, exps, pcnt, divs, &dcnt);

    /* --- 4. Sort the divisor list --------------------------------------- */
    qsort(divs, dcnt, sizeof(uint64_t), cmp_uint64);

    /* --- 5. Count prime divisors with a larger divisor ------------------- */
    uint64_t count = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        if (!is_prime_small(divs[i])) {
            continue;
        }
        /* All later divisors are larger because the list is sorted. */
        count += (uint64_t)(dcnt - i - 1);
    }

    free(divs);
    return (seq_t)count;
}
