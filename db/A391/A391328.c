/*
 * A391328  Odd semiprimes k = p*q, p, q primes > 3, such that either k =
 *          A048720(p,x) or k = A048720(q,x) for some x, where A048720 is the
 *          carryless binary multiplication.
 * https://oeis.org/A391328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1736, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = first n odd semiprimes k = p*q (p,q>3) such that
 * either k is the carryless binary product of p and some integer, or
 * it is the carryless binary product of q and some integer.
 * The method enumerates odd numbers, tests semiprimality, and checks
 * the carryless divisibility condition via polynomial remainder.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper for computing bit length of a 64‑bit unsigned integer. */
static inline unsigned bitlen_uint64(uint64_t x) {
    return x ? 64 - __builtin_clzll(x) : 0;
}

/* remainder of a divided by p in GF(2) (carryless binary division). */
static inline uint64_t rem_carryless(uint64_t a, uint64_t p) {
    if (p == 0) return a;
    uint64_t r = a;
    while (p <= r) {
        unsigned shift = bitlen_uint64(r) - bitlen_uint64(p);
        r ^= p << shift;
    }
    return r;
}

/* ------------------------------------------------------------ */
/* Global state for the sequence generator.                    */
/* ------------------------------------------------------------ */
static bool seq_initialized = false;
static uint32_t *spf = NULL;          /* smallest prime factor array */
static size_t spf_limit = 0;          /* current sieve limit */
static seq_t *seq_vals = NULL;        /* cached sequence values */
static size_t seq_len = 0;            /* number of cached values */
static size_t seq_cap = 0;            /* capacity of seq_vals array */
static idx_t next_scan = 35;          /* next candidate number to test */

/* ------------------------------------------------------------ */
/* Sieve of Eratosthenes + spf table.                         */
/* ------------------------------------------------------------ */
static void init_sequence(void) {
    if (seq_initialized) return;

    /* Sieve limit big enough to cover at least 10,000 terms.       */
    /* 4 000 000 yields about 200 000 odd numbers – ample density. */
    const idx_t LIMIT = 4000000;
    spf_limit = (size_t)LIMIT;
    spf = (uint32_t*)malloc((spf_limit + 1) * sizeof(uint32_t));
    for (size_t i = 0; i <= spf_limit; ++i) spf[i] = 0;

    for (size_t i = 2; i <= spf_limit; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = (uint32_t)i;
            if (i <= spf_limit / i) {
                for (size_t j = i * i; j <= spf_limit; j += i) {
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }
    }

    seq_vals = NULL;
    seq_len  = 0;
    seq_cap  = 0;
    next_scan = 35;

    seq_initialized = true;
}

/* ------------------------------------------------------------ */
/* Append a new term to the cached sequence.                   */
/* ------------------------------------------------------------ */
static void append_term(seq_t val) {
    if (seq_len == seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 32 : seq_cap * 2;
        seq_vals = (seq_t*)realloc(seq_vals, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
    seq_vals[seq_len++] = val;
}

/* ------------------------------------------------------------ */
/* Main sequence function.                                     */
/* ------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined; return 0 for safety */

    init_sequence();

    while (seq_len < (size_t)n) {
        /* Scan odd numbers starting from next_scan. */
        for (idx_t m = next_scan; m <= (idx_t)spf_limit; ++m) {
            if ((m & 1) == 0 || m <= 3) continue;  /* skip evens & small */

            uint32_t p = spf[(size_t)m];
            if (p <= 3) continue;                  /* first factor must >3 */

            uint64_t q = (uint64_t)m / (uint64_t)p;
            if (q <= 3) continue;                  /* second factor must >3 */
            if (spf[(size_t)q] != (uint32_t)q) continue;  /* q must be prime */

            /* Check carryless divisibility. */
            if (rem_carryless((uint64_t)m, (uint64_t)p) == 0 ||
                rem_carryless((uint64_t)m, (uint64_t)q) == 0) {
                append_term((seq_t)m);
                next_scan = m + 1;
                break;  /* go to next term */
            }
        }
    }

    return seq_vals[n - 1];
}
