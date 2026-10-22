/*
 * A350950  Number of even parts minus number of even conjugate parts in the
 *          integer partition with Heinz number n.
 * https://oeis.org/A350950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (# of even parts) – (# of even parts in the conjugate
 *            partition) computed from the partition whose Heinz number is n
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   build_primes, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#include "../utils/factor_u64.h"
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    /* special case: the Heinz number 1 corresponds to the empty partition */
    if (n == 1) return (seq_t)0;

    /* ---------- 1. factorize n  ------------------------------------------- */
    /* maximum number of distinct prime factors for n <= 2^64 is < 20 */
    uint64_t fac_pr[64];
    uint64_t fac_exp[64];
    size_t fac_cnt = 0;
    factor_u64(n, fac_pr, fac_exp, &fac_cnt);
    if (fac_cnt == 0)        /* n < 2, should never happen except n==1 above */
        return (seq_t)0;

    /* ---------- 2. generate primes up to the largest prime factor  --------- */
    idx_t max_prime = fac_pr[0];
    for (size_t i = 1; i < fac_cnt; ++i)
        if (fac_pr[i] > max_prime) max_prime = fac_pr[i];

    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    /* build_primes allocates an array of primes <= limit and assigns it to 'primes' */
    build_primes(max_prime, &primes, &prime_cnt);
    if (prime_cnt == 0) /* failure to allocate */
        return (seq_t)0;

    /* ---------- 3. map each prime factor to its index --------------------- */
    /* For each prime factor p, find its 1‑based position in the primes list */
    size_t *part_counts = NULL;   /* array indexed by part value (prime index)   */
    size_t max_index = 0;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p = fac_pr[i];
        /* linear search – primes are few (<= ~1229 for max_prime <= 10000) */
        size_t pos = 0;
        for (size_t j = 0; j < prime_cnt; ++j) {
            if (primes[j] == p) { pos = j + 1; break; }
        }
        if (pos == 0) continue;    /* should not happen */
        if (pos > max_index) max_index = pos;
    }
    part_counts = (size_t *)calloc((size_t)(max_index + 1), sizeof(size_t));
    if (!part_counts) { free(primes); return (seq_t)0; }

    /* fill counts */
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p = fac_pr[i];
        size_t pos = 0;
        for (size_t j = 0; j < prime_cnt; ++j) {
            if (primes[j] == p) { pos = j + 1; break; }
        }
        if (pos == 0) continue;
        part_counts[pos] += (size_t)fac_exp[i];
    }

    /* ---------- 4. count even parts --------------------------------------- */
    size_t even_parts = 0;
    for (size_t k = 2; k <= max_index; k += 2)
        even_parts += part_counts[k];

    /* ---------- 5. count even parts in the conjugate partition ------------- */
    size_t even_conj = 0;
    size_t cumulative = 0;
    for (size_t i = max_index; i >= 1; --i) {
        cumulative += part_counts[i];
        if ((cumulative & 1ULL) == 0) even_conj++;
    }

    /* ---------- 6. clean up ----------------------------------------------- */
    free(primes);
    free(part_counts);

    /* ---------- 7. return difference --------------------------------------- */
    seq_t result = (seq_t)((seq_t)even_parts - (seq_t)even_conj);
    return result;
}
