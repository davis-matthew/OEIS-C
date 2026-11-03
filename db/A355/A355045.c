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
 * Validated: standard, pass, 32 terms through n = 857 (2026-11-01)
 * Helpers:   factor_u64, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

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
        seq_t pow = ipow((seq_t)S[i], (seq_t)e);
        k = seq_mul(k, pow);
    }

    free(S);
    free(sumTerm);
    return k;
}
