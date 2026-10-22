/*
 * A355045  a(n) is the least positive integer k which is a multiple of
 *          prime(n) such that for some m >= 0, psi(k) = rad(k)^m, where
 *          psi(k) = A001615(k) and rad(k) = A007947(k).
 * https://oeis.org/A355045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Minimal k multiple of prime(n) with ψ(k)=rad(k)^m
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, factor_u64, get_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Get the n‑th prime (1‑indexed). */
    uint32_t p = get_prime(n);
    /* Dynamic array for the closure set S. */
    size_t cap = 64;
    uint32_t *S   = (uint32_t*)malloc(cap * sizeof(uint32_t));
    size_t  sz    = 0;
    S[sz++] = p;
    bool changed = true;

    /* Build closure: add primes dividing (q+1) for each q in S until no change. */
    while (changed) {
        changed = false;
        for (size_t i = 0; i < sz; ++i) {
            uint64_t val = (uint64_t)S[i] + 1ULL;
            uint64_t pr[64], ex[64];
            size_t cnt = 0;
            factor_u64(val, pr, ex, &cnt);
            for (size_t j = 0; j < cnt; ++j) {
                uint32_t f = (uint32_t)pr[j];
                /* Check if f already in S. */
                bool exists = false;
                for (size_t k = 0; k < sz; ++k) {
                    if (S[k] == f) { exists = true; break; }
                }
                if (!exists) {
                    if (sz == cap) {
                        cap <<= 1;
                        S = (uint32_t*)realloc(S, cap * sizeof(uint32_t));
                    }
                    S[sz++] = f;
                    changed = true;
                }
            }
        }
    }

    /* Compute sumTerm for each prime in S. */
    size_t *sumTerm = (size_t*)calloc(sz, sizeof(size_t));
    for (size_t qi = 0; qi < sz; ++qi) {
        uint32_t q = S[qi];
        for (size_t pi = 0; pi < sz; ++pi) {
            uint64_t val = (uint64_t)S[pi] + 1ULL;
            uint64_t pr[64], ex[64];
            size_t cnt = 0;
            factor_u64(val, pr, ex, &cnt);
            for (size_t j = 0; j < cnt; ++j) {
                if ((uint32_t)pr[j] == q) {
                    sumTerm[qi] += ex[j];
                }
            }
        }
    }

    /* Find maximal sumTerm => minimal possible m. */
    size_t maxSum = 0;
    for (size_t i = 0; i < sz; ++i) {
        if (sumTerm[i] > maxSum) maxSum = sumTerm[i];
    }
    size_t m = maxSum;

    /* Compute result k = product_{q∈S} q^{e_q}. */
    seq_t k = (seq_t)1;
    for (size_t i = 0; i < sz; ++i) {
        size_t e = (m + 1) - sumTerm[i];
        seq_t pow = big_pow((seq_t)S[i], (seq_t)e);
        k = seq_mul(k, pow);
    }

    free(S);
    free(sumTerm);
    return k;
}
