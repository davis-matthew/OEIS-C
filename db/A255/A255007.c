/*
 * A255007  a(n) is the denominator of polygamma(2n+1, 1) / Pi^(2n+2).
 * https://oeis.org/A255007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of polygamma(2n+1,1)/π^(2n+2)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   int_log2_uint64, odd_part_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The denominator can be computed from the odd primes p satisfying
 *   p-1 | (2n+2)
 * Let D_odd be the product of those primes.  For each n, let
 * odd_part(n+1) be (n+1) with all 2‑factors removed.  Then
 * a(n) = D_odd * odd_part(n+1) divided by all prime factors of
 * odd_part(n+1) that are NOT in D_odd.  (This removes the
 * unnecessary odd factors that are not required in the final
 * denominator.)  Special case: a(0)=6.
 *
 * The function recomputes the list of primes up to 2n+2 only when a
 * larger limit is requested.  All arithmetic is performed with seq_t,
 * which may be an integer type of arbitrary width or an
 * arbitrary‑precision integer class.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/int_log2_uint64.h"
#include "../utils/odd_part_uint64.h"

static uint64_t *primes = NULL;   /* array of primes */
static size_t  prime_cnt = 0;     /* number of primes stored */
static uint64_t prime_limit = 0;  /* maximum index for which primes are known */

static void ensure_primes(uint64_t limit)
{
    if (limit <= prime_limit) return;   /* already available */

    /* free previous allocation */
    free(primes);

    /* new limits */
    prime_limit = limit;
    primes      = (uint64_t *)malloc((prime_limit + 1) * sizeof(uint64_t));

    bool *is_comp = (bool *)calloc(prime_limit + 1, sizeof(bool));
    prime_cnt = 0;
    for (uint64_t i = 2; i <= prime_limit; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = i;
            if (i * i <= prime_limit) {
                for (uint64_t j = i * i; j <= prime_limit; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)6;               /* special case */

    /* m = 2n+2 is the index in the Bernoulli number denominator formula  */
    uint64_t m = 2ULL * (uint64_t)n + 2ULL;

    /* ensure we have all primes up to m+1 for the following tests */
    ensure_primes(m + 1ULL);

    /* Compute D_odd: product of odd primes p such that (p-1) divides m   */
    seq_t D_odd = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p == 2ULL) continue;          /* skip even prime */
        if (m % (p - 1ULL) == 0ULL)
            D_odd *= (seq_t)p;
    }

    /* odd part of n+1 */
    uint64_t n1 = (uint64_t)n + 1ULL;
    uint64_t odd = odd_part_uint64(n1);

    /* candidate value before removing unnecessary odd factors */
    seq_t candidate = D_odd * (seq_t)odd;

    /* factorize odd and remove primes not in D_odd */
    uint64_t temp = odd;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0ULL) {
            size_t exp = 0;
            while (temp % p == 0ULL) {
                temp /= p;
                ++exp;
            }
            /* test whether p belongs to D_odd   */
            if (m % (p - 1ULL) != 0ULL) {
                seq_t factor = 1;
                for (size_t k = 0; k < exp; ++k)
                    factor *= (seq_t)p;
                candidate /= factor;          /* p was not needed */
            }
        }
    }
    if (temp > 1ULL) {                        /* remaining prime factor */
        uint64_t p = temp;
        if (m % (p - 1ULL) != 0ULL)
            candidate /= (seq_t)p;
    }

    return candidate;
}
