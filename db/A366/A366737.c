/*
 * A366737  Number of numbers k <= A056239(n) that can be written as a linear
 *          combination of the prime indices of n (allowing coefficients of
 *          0).
 * https://oeis.org/A366737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A366737: Number of k ≤ A056239(n) that can be written as a linear
   combination of the distinct prime indices of n (coefficients ≥ 0). */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  Static data, initialised lazily on first call                        */
/* --------------------------------------------------------------------- */
static bool prime_table_ready = false;     /* flag */
static size_t prime_count    = 0;          /* number of primes found */
static uint64_t *primes      = NULL;       /* array of primes */
static idx_t    *prime_index = NULL;       /* prime_index[p] = position of p (1‑based) */

/* --------------------------------------------------------------------- */
/*  Simple sieve to build primes up to LIMIT (inclusive)                 */
/* --------------------------------------------------------------------- */
static void build_primes_to_limit(idx_t limit)
{
    /* Allocate boolean marking array */
    bool *is_prime = (bool*)calloc((size_t)limit + 1, sizeof(bool));
    if (!is_prime) return; /* allocation failure; unlikely */

    for (size_t i = 2; i <= limit; ++i) is_prime[i] = true;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i) is_prime[j] = false;
        }
    }

    /* Count primes and allocate array */
    prime_count = 0;
    for (size_t i = 2; i <= limit; ++i) if (is_prime[i]) ++prime_count;
    primes = (uint64_t*)malloc(prime_count * sizeof(uint64_t));
    prime_index = (idx_t*)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!primes || !prime_index) { free(is_prime); return; }

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) {
        prime_index[i] = 0;                /* initialise mapping */
        if (is_prime[i]) {
            primes[idx] = (uint64_t)i;
            prime_index[i] = (idx_t)(idx + 1);  /* 1‑based index */
            ++idx;
        }
    }

    free(is_prime);
    prime_table_ready = true;
}

/* --------------------------------------------------------------------- */
/*  Function computes A366737 for a given n                              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* 0‑based arguments are not used; we return 0 for n < 1.          */
    if (n < 1) return (seq_t)0;

    /* Initialise primes once, with a reasonable limit.               */
    if (!prime_table_ready) {
        /* 1 000 000 is enough for n up to ~10^12. Adjust if needed.   */
        build_primes_to_limit(1000000ULL);
        if (!prime_table_ready) return (seq_t)0; /* failed initialisation */
    }

    /* ------------------------------------------------------------------ */
    /*  Factor n, collect distinct prime indices, and compute S = A056239(n) */
    /* ------------------------------------------------------------------ */
    idx_t temp = n;
    idx_t indices[64];          /* number of distinct prime factors is tiny */
    size_t idx_cnt = 0;
    idx_t S = 0;

    /* Trial division using pre‑computed primes. */
    for (size_t p_idx = 0; p_idx < prime_count && (uint64_t)primes[p_idx] * (uint64_t)primes[p_idx] <= (uint64_t)temp; ++p_idx) {
        uint64_t p = primes[p_idx];
        if (temp % p == 0) {
            /* Count exponent. */
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }

            /* Prime index is 1‑based position in primes array. */
            idx_t w = prime_index[p];  /* guaranteed non‑zero here */
            S += (idx_t)exp * w;

            /* Store distinct weight once. */
            bool seen = false;
            for (size_t i = 0; i < idx_cnt; ++i) {
                if (indices[i] == w) { seen = true; break; }
            }
            if (!seen) indices[idx_cnt++] = w;
        }
    }

    /* Remaining factor > 1 (if any) is prime. */
    if (temp > 1) {
        uint64_t p = temp;
        /* Find its index.  If p exceeds our sieve limit, use binary search. */
        idx_t w;
        if (p <= (uint64_t)prime_count) { /* unlikely but safe */
            w = prime_index[p];
        } else {
            /* Binary search in primes array */
            size_t lo = 0, hi = prime_count;
            while (lo < hi) {
                size_t mid = (lo + hi) / 2;
                if (primes[mid] < p) lo = mid + 1;
                else hi = mid;
            }
            /* lo gives position of p if present. */
            w = (idx_t)(lo + 1);
        }
        S += w;  /* exponent 1 */
        bool seen = false;
        for (size_t i = 0; i < idx_cnt; ++i) {
            if (indices[i] == w) { seen = true; break; }
        }
        if (!seen) indices[idx_cnt++] = w;
    }

    if (S == 0) return (seq_t)0;  /* n = 1 case */

    /* ------------------------------------------------------------------ */
    /*  Unbounded knapsack DP: Mark all representable sums ≤ S.           */
    /* ------------------------------------------------------------------ */
    size_t size = (size_t)S + 1;
    bool *dp = (bool*)calloc(size, sizeof(bool));
    if (!dp) return (seq_t)0;  /* allocation failure – unlikely */

    dp[0] = true;
    for (size_t i = 0; i < idx_cnt; ++i) {
        idx_t w = indices[i];
        for (size_t j = (size_t)w; j <= S; ++j) {
            if (dp[j - (size_t)w]) dp[j] = true;
        }
    }

    /* Count k from 1 to S that are representable. */
    seq_t result = 0;
    for (size_t k = 1; k <= S; ++k) if (dp[k]) ++result;

    free(dp);
    return result;
}
