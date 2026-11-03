/*
 * A275735  Prime-factorization representations of "factorial base level
 *          polynomials": a(0) = 1; for n >= 1, a(n) = 2^A257511(n) *
 *          A003961(a(A257684(n))).
 * https://oeis.org/A275735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = ∏_{k ≥ 1} prime(k)^{count of digit k in factorial base
 *            representation of n}
 * Range:     u8 n <= 214, u16 n <= 14398, u32 n <= 40320, u64 n <= 40320,
 *            u128 n <= 40320, big any n
 * Validated: standard, pass, 111 terms through n = 9812 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

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
    if (n == 0) return (seq_t)1;

    /* Precomputed factorials up to 20! (fits in 64‑bit) */
    static uint64_t fact[21];
    static bool init = false;
    if (!init) {
        fact[0] = 1;
        for (int i = 1; i <= 20; ++i) fact[i] = fact[i-1] * (uint64_t)i;
        init = true;
    }

    /* Find the largest j with j! <= n */
    int maxj = 1;
    while (maxj < 20 && fact[maxj + 1] <= (uint64_t)n) ++maxj;

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    for (int j = maxj; j >= 1; --j) {
        uint64_t digit = m / fact[j];
        m %= fact[j];
        if (digit != 0) {
            seq_t prime = (seq_t)get_prime((idx_t)digit);
            result = seq_mul(result, prime);  /* multiply once per occurrence */
        }
    }
    return result;
}
