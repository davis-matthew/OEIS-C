/*
 * A282043  Let p = n-th prime == 7 mod 8; a(n) = sum of quadratic nonresidues
 *          mod p.
 * https://oeis.org/A282043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of quadratic nonresidues modulo the n-th prime ≡ 7
 *            (mod 8)
 * Range:     u8 n <= 2, u16 n <= 25, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

#if defined(__cplusplus)
    static uint64_t *primes7 = nullptr;
    static seq_t *sums    = nullptr;
#else
    static uint64_t *primes7 = NULL;
    static seq_t *sums       = NULL;
#endif
static size_t primes_cnt = 0;
static size_t primes_cap = 0;
static size_t sums_cnt   = 0;
static size_t sums_cap   = 0;
static uint64_t next_candidate = 7;

/* Ensure we have at least n primes ≡ 7 (mod 8) and their sums computed */
static void ensure_cache(idx_t n)
{
    while (primes_cnt < (size_t)n) {
        if (is_prime((idx_t)next_candidate)) {
            /* grow primes array if needed */
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap == 0 ? 16 : primes_cap * 2;
#if defined(__cplusplus)
                uint64_t *tmp = new uint64_t[new_cap];
#else
                uint64_t *tmp = (uint64_t *)realloc(primes7, new_cap * sizeof(uint64_t));
                if (!tmp) abort();
#endif
                primes7 = tmp;
                primes_cap = new_cap;
            }
            primes7[primes_cnt++] = next_candidate;

            uint64_t p = next_candidate;
            uint64_t half = (p - 1) / 2;
            seq_t residue_sum = 0;
            for (uint64_t a = 1; a <= half; ++a) {
                uint64_t sq = (a * a) % p;
                residue_sum += (seq_t)sq;
            }
            seq_t total = ((seq_t)p * (seq_t)(p - 1)) / 2;
            seq_t nonresidue_sum = total - residue_sum;

            /* grow sums array if needed */
            if (sums_cnt == sums_cap) {
                size_t new_cap = sums_cap == 0 ? 16 : sums_cap * 2;
#if defined(__cplusplus)
                seq_t *tmp_sum = new seq_t[new_cap];
#else
                seq_t *tmp_sum = (seq_t *)realloc(sums, new_cap * sizeof(seq_t));
                if (!tmp_sum) abort();
#endif
                sums = tmp_sum;
                sums_cap = new_cap;
            }
            sums[sums_cnt++] = nonresidue_sum;
        }
        next_candidate += 8;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_cache(n);
    return sums[n - 1];
}
