/*
 * A360017  Nonsquarefree numbers k such that k - d is also a nonsquarefree
 *          number for all proper divisors d of k.
 * https://oeis.org/A360017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth element of A360017. * Computed by a brute‑force
 *            search of nonsquarefree integers k such that * for every proper
 *            divisor d of k, the integer k-d is also nonsquarefree. * The
 *            sequence is generated on demand and memoised; it works for any *
 *            n ≥ 1 and is efficient enough to compute the fir...
 * Range:     u8 n <= 5, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* -------------------------------------------------------------------- */
/* Global static state used by the generation routine                   */
/* -------------------------------------------------------------------- */
static idx_t   sieve_limit = 0;          /* upper bound of current sieve    */
static uint32_t *spf            = NULL; /* smallest prime factor array     */
static uint8_t  *is_squarefree  = NULL; /* 1 = squarefree, 0 = not         */

static seq_t   *seq          = NULL; /* array of generated terms         */
static size_t  seq_len       = 0;    /* current number of terms          */
static size_t  seq_cap       = 0;    /* capacity of seq array            */

static idx_t   next_candidate = 1;    /* next integer to test             */

/* -------------------------------------------------------------------- */
/* Rebuild the sieve data structures up to 'limit' (inclusive).        */
/* Uses a linear sieve for spf and marks multiples of p² for squarefree.*/
/* -------------------------------------------------------------------- */
static void rebuild_sieve(idx_t limit) {
    if (limit <= sieve_limit) return;

    /* Allocate new arrays */
    uint32_t *new_spf = (uint32_t *)calloc(limit + 1U, sizeof(uint32_t));
    uint8_t  *new_sf  = (uint8_t  *)calloc(limit + 1U, sizeof(uint8_t  ));
    if (!new_spf || !new_sf) exit(EXIT_FAILURE);

    /* Rough capacity estimate for the list of primes */
    size_t primes_cap = limit / 10U + 1000U;
    uint32_t *primes  = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);
    size_t prime_cnt = 0U;

    /* Linear sieve to fill spf */
    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = (uint32_t)i;
            if (prime_cnt < primes_cap) primes[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes[j];
            if (p > new_spf[i]) break;
            uint64_t v = (uint64_t)i * p;
            if (v > (uint64_t)limit) break;
            new_spf[(idx_t)v] = p;
        }
    }

    /* Mark numbers that are NOT squarefree */
    memset(new_sf, 1, (size_t)(limit + 1U));
    for (size_t j = 0; j < prime_cnt; ++j) {
        uint64_t p2 = (uint64_t)primes[j] * (uint64_t)primes[j];
        if (p2 > (uint64_t)limit) break;
        for (uint64_t m = p2; m <= (uint64_t)limit; m += p2)
            new_sf[(idx_t)m] = 0;
    }

    /* Replace old tables */
    free(spf);
    free(is_squarefree);
    free(primes);
    spf           = new_spf;
    is_squarefree = new_sf;
    sieve_limit   = limit;
}

/* -------------------------------------------------------------------- */
/* Test whether the candidate 'k' satisfies the defining property.      */
/* -------------------------------------------------------------------- */
static bool check_property(uint32_t k) {
    /* Factor 'k' using the spf table */
    uint32_t primes[32];
    uint32_t exps[32];
    size_t   dcnt = 0U;
    uint32_t tmp  = k;
    while (tmp > 1) {
        uint32_t p = spf[tmp];
        uint32_t e = 0U;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        primes[dcnt] = p;
        exps[dcnt]  = e;
        ++dcnt;
    }

    /* Generate all divisors of 'k' (including 1 and k) */
    uint32_t divs[4096];
    size_t   divcnt = 1U;
    divs[0] = 1U;
    for (size_t i = 0; i < dcnt; ++i) {
        uint32_t p = primes[i];
        uint32_t e = exps[i];
        size_t   oldcnt = divcnt;
        for (size_t idx = 0; idx < oldcnt; ++idx) {
            uint32_t base = divs[idx];
            uint32_t mult = 1U;
            for (uint32_t j = 1; j <= e; ++j) {
                mult *= p;
                if (divcnt < sizeof(divs)/sizeof(divs[0]))
                    divs[divcnt++] = base * mult;
            }
        }
    }

    /* Check the property */
    for (size_t i = 0; i < divcnt; ++i) {
        uint32_t d = divs[i];
        if (d == k) continue;
        uint32_t diff = k - d;
        if (is_squarefree[diff])  /* k-d is squarefree -> fail */
            return false;
    }
    return true;
}

/* -------------------------------------------------------------------- */
/* The required function.  Returns the nth term (1‑based indexing).     */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;    /* unused but safe */

    /* Lazy initialisation */
    if (!seq) {
        /* Allocate some initial memory for the sequence */
        seq_cap = 16;
        seq     = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) exit(EXIT_FAILURE);
        seq_len = 0;
        rebuild_sieve(4096U);  /* start with a modest sieve size */
    }

    /* Generate terms until we have at least 'n' of them */
    while (seq_len < (size_t)n) {
        ++next_candidate;
        if ((idx_t)next_candidate > sieve_limit)
            rebuild_sieve((idx_t)next_candidate * 2U + 1000U);

        /* Only nonsquarefree numbers can qualify */
        if (is_squarefree[(uint32_t)next_candidate])
            continue;

        if (check_property((uint32_t)next_candidate)) {
            /* Append to our sequence */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2U : 16U;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                if (!seq) exit(EXIT_FAILURE);
            }
            seq[seq_len++] = (seq_t)next_candidate;
        }
    }

    /* Return the requested term */
    return seq[(size_t)n - 1U];
}
