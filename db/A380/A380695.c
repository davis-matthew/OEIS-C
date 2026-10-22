/*
 * A380695  Numbers k such that the prime index of the least prime dividing k
 *          is larger than or equal to the maximum exponent in the prime
 *          factorization of k; a(1) = 1 by convention.
 * https://oeis.org/A380695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 175, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A380695 – Numbers k such that the prime index of the least prime
 * dividing k is ≥ the maximum exponent in k's prime factorisation.
 * The implementation pre‑computes a least‑prime‑factor sieve up to a
 * large bound and then generates terms by incremental scanning.
 * Results are cached so that any order of calls is efficient. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* --- Parameters ------------------------------------------------------ */
static const idx_t LIMIT = 1000000ULL;   /* sieve limit (≥ expected k for n<10000) */

/* --- Global data ----------------------------------------------------- */
static uint32_t *spf = NULL;          /* least prime factor array */
static uint32_t *prime_pi = NULL;     /* prime index (π(p)) array */

static seq_t *seq_cache = NULL;       /* cached sequence values */
static size_t seq_len = 0;            /* number of terms stored */
static size_t seq_cap = 0;            /* allocated capacity of seq_cache */
static uint64_t next_k = 1;           /* next integer to test */

/* --- Helper: initialise sieve once ---------------------------------- */
static void init_sieve(void)
{
    if (spf != NULL) return;           /* already initialised */

    /* Allocate and zero arrays – cast to silence C++ conversion error */
    spf = (uint32_t*)calloc((size_t)(LIMIT + 1), sizeof(uint32_t));
    prime_pi = (uint32_t*)calloc((size_t)(LIMIT + 1), sizeof(uint32_t));

    /* Simple least‑prime‑factor sieve. */
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {             /* i is prime */
            for (idx_t j = i; j <= LIMIT; j += i) {
                if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }

    /* Compute prime index table. */
    idx_t cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == (uint32_t)i) {    /* i is prime */
            ++cnt;
            prime_pi[i] = (uint32_t)cnt;
        }
    }
}

/* --- Helper: test whether k satisfies the condition ------------------ */
static bool check_value(uint64_t k)
{
    if (k == 1) return true;           /* by convention */

    if (k > LIMIT) {
        /* For the intended range (n ≤ 10000) this should never happen. */
        return false;
    }

    uint64_t tmp = k;
    uint32_t lpf = spf[(size_t)k];
    uint32_t pi_lpf = prime_pi[(size_t)lpf];
    uint32_t e_max = 0;

    while (tmp > 1) {
        uint32_t p = spf[(size_t)tmp];
        uint32_t e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        if (e > e_max) e_max = e;
    }
    return pi_lpf >= e_max;
}

/* --- Main sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure global data are initialised. */
    init_sieve();

    /* Allocate cache on first call. */
    if (seq_cache == NULL) {
        seq_cap = 1024;
        seq_cache = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
        next_k = 1;
    }

    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        uint64_t k = next_k;
        if (check_value(k)) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_cache = (seq_t*)realloc(seq_cache, seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = k;   /* implicit conversion to seq_t */
        }
        ++next_k;
    }
    return seq_cache[n - 1];
}
