/*
 * A354722  Composite numbers whose divisors have distinct binary weights
 *          (A000120).
 * https://oeis.org/A354722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A354722 – Composite numbers whose divisors have distinct binary weights
   (odd composite numbers such that the set of Hamming‐weights of all divisors is
   pairwise distinct).  The function a(idx_t n) returns the n-th such number
   (a(1)=25).  The implementation uses a simple incremental search with
   trial‑division factorisation, which is sufficient for n up to at least 10000.
   No external utilities are required – the code is self‑contained.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Forward declarations for the helper functions that operate on uint64_t. */
static bool is_prime_simple(uint64_t n);
static size_t factor_simple(uint64_t n, uint64_t *primes, int *exps);
static bool distinct_popcounts(uint64_t n);
static void ensure_capacity(seq_t **arr, size_t *cap, size_t need);

/* --------------------------------------------------------------------- */
/*   Main function required by the build system                         */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;   /* cached sequence */
    static size_t term_cap = 0;
    static size_t term_len = 0;
    static uint64_t last_checked = 0;

    /* Populate the cache until it contains at least n terms. */
    while (term_len < n) {
        uint64_t candidate;
        if (last_checked == 0) {
            candidate = 25;          /* first odd composite candidate */
        } else {
            candidate = last_checked + 2; /* skip evens */
        }

        /* Find next candidate that satisfies the property. */
        while (!distinct_popcounts(candidate)) {
            candidate += 2;
        }

        /* Store the candidate in the cache. */
        ensure_capacity(&terms, &term_cap, term_len + 1);
        terms[term_len++] = (seq_t)candidate;
        last_checked = candidate;
    }

    return terms[n - 1];
}

/* --------------------------------------------------------------------- */
/*  Helper: Prime test by trial division (sufficient for numbers < 10^10). */
static bool is_prime_simple(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/*  Helper: Prime factorisation (trial division).  Assumes n > 1.        */
static size_t factor_simple(uint64_t n, uint64_t *primes, int *exps)
{
    size_t cnt = 0;
    if (n % 2 == 0) {
        primes[cnt] = 2;
        exps[cnt] = 0;
        while (n % 2 == 0) { n /= 2; exps[cnt]++; }
        cnt++;
    }
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            primes[cnt] = p;
            exps[cnt] = 0;
            while (n % p == 0) { n /= p; exps[cnt]++; }
            cnt++;
        }
    }
    if (n > 1) {            /* remaining prime factor */
        primes[cnt] = n;
        exps[cnt] = 1;
        cnt++;
    }
    return cnt;
}

/* --------------------------------------------------------------------- */
/*  Helper: Generate all divisors and check that all have distinct     */
/*          binary Hamming weights.                                      */
static bool distinct_popcounts(uint64_t n)
{
    /* Must be odd composite. */
    if (n <= 2 || (n % 2 == 0) || is_prime_simple(n)) return false;

    /* Factor n. */
    uint64_t primes[64];
    int     exps[64];
    size_t  pcnt = factor_simple(n, primes, exps);

    /* Prepare buffer for divisors; maximum count for 64‑bit is small. */
    uint64_t divs[4096];
    size_t   dcnt = 1;
    divs[0] = 1;

    /* Generate divisors by expanding with each prime power. */
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        int     e = exps[i];
        size_t   cur = dcnt;
        uint64_t pow = 1;
        for (int k = 1; k <= e; ++k) {
            pow *= p;
            for (size_t j = 0; j < cur; ++j) {
                divs[dcnt++] = divs[j] * pow;
            }
        }
    }

    /* Check that all binary weights are distinct. Assume weight <= 64. */
    bool used[65] = {0};

    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d   = divs[i];
        unsigned w   = (unsigned)__builtin_popcountll(d);
        if (used[w]) return false;
        used[w] = true;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/*  Helper: Resize cached sequence array.                               */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* allocation failed */
    *arr = tmp;
    *cap = new_cap;
}
