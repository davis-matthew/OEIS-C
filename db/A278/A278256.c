/*
 * A278256  Least number with the same prime signature as the n-th oblong
 *          number (A002378).
 * https://oeis.org/A278256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least integer having the same prime signature as the n‑th
 *            oblong number n·(n+1). The "prime signature" is the multiset of
 *            exponents in the prime factorisation. “Least integer with that
 *            signature” is obtained by assigning the largest exponents to the
 *            smallest primes
 * Range:     u8 n <= 19, u16 n <= 383, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"
#include "../utils/pow_uint.h"

/* ------------------------------------------------------------------
 * Prime generation ---------------------------------------------------
 * ------------------------------------------------------------------ */

/* Holds the first 256 primes.  256 is more than enough for exponents
   coming from n ≤ 10 000. */
static uint32_t primes_arr[256];
static size_t    primes_cnt = 0;

/* Initialise primes_arr with the first 256 primes (if not already done). */
static void init_primes(void)
{
    if (primes_cnt > 0)
        return;                     /* already initialised */

    primes_arr[0] = 2;
    primes_cnt   = 1;

    for (uint32_t candidate = 3; primes_cnt < 256; candidate += 2) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t p = primes_arr[i];
            if ((uint64_t)p * p > (uint64_t)candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            primes_arr[primes_cnt++] = candidate;
    }
}

/* Ensure that we have at least 'needed' primes in primes_arr. */
static void ensure_prime_count(size_t needed)
{
    if (primes_cnt >= needed)
        return;
    init_primes();                    /* will always create 256 primes */
}

/* ------------------------------------------------------------------
 * Factorisation ------------------------------------------------------
 * ------------------------------------------------------------------ */

/* Factorise m (≤ 2^64) and store the exponents in exps_out.
   Returns the number of exponents written. */
static size_t factor_uint64(uint64_t m, uint32_t *exps_out)
{
    size_t idx = 0;

    /* factor 2 */
    uint32_t e = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++e;
    }
    if (e > 0) {
        exps_out[idx++] = e;
        e = 0;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        if (e > 0)
            exps_out[idx++] = e;
    }

    /* remaining prime > 1 */
    if (m > 1)
        exps_out[idx++] = 1;

    return idx;
}

/* ------------------------------------------------------------------
 * Helper for sorting exponents in descending order ------------------
 * ------------------------------------------------------------------ */
static int cmp_desc(const void *a, const void *b)
{
    const uint32_t *x = (const uint32_t *)a;
    const uint32_t *y = (const uint32_t *)b;
    if (*y > *x) return 1;
    if (*y < *x) return -1;
    return 0;
}

/* ------------------------------------------------------------------
 * Main sequence function ---------------------------------------------
 * ------------------------------------------------------------------ */

/* External linkage as required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                         /* not defined for n=0 */

    /* factor n and n+1 separately */
    uint32_t exps1[64], exps2[64];
    size_t cnt1 = factor_uint64((uint64_t)n, exps1);
    size_t cnt2 = factor_uint64((uint64_t)(n + 1), exps2);

    /* combine the exponent lists */
    uint32_t exps_combined[128];
    size_t cnt_combined = 0;
    for (size_t i = 0; i < cnt1; ++i)
        exps_combined[cnt_combined++] = exps1[i];
    for (size_t i = 0; i < cnt2; ++i)
        exps_combined[cnt_combined++] = exps2[i];

    /* sort descending (largest exponents first) */
    qsort(exps_combined, cnt_combined, sizeof(uint32_t), cmp_desc);

    /* make sure we have enough primes */
    ensure_prime_count(cnt_combined);

    /* compute the minimal number with this signature */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt_combined; ++i) {
        seq_t term = pow_uint((uint64_t)primes_arr[i], (idx_t)exps_combined[i]);
        result = seq_mul(result, term);
    }

    return result;
}
