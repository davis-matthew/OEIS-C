/*
 * A349079  Numbers k such that there exists m, 1 <= m <= k with the property
 *          that A000178(k) / m! is a square, where A000178(k) = k$ =
 *          1!*2!*...*k! is the superfactorial of k.
 * https://oeis.org/A349079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   bitset_copy, bitset_equal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates OEIS sequence A349079: numbers k such that k$ / m! is a square
   for some 1 ≤ m ≤ k, where k$ = 1! · 2! · … · k!.  The test is performed using the
   parity (mod 2) of exponents of all primes in the factorials.
   The algorithm keeps a running parity vector for each factorial and for the
   cumulative super‑factorial, checks whether the current parity vector appears
   among the candidate m‑values and stores all k that satisfy the condition.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/bitset_copy.h"
#include "../utils/bitset_equal.h"

/* --------------------------------------------------------------------- */
/*  Configuration: limit for precomputation of factorial parities         */
/* --------------------------------------------------------------------- */

#define MAXK 100000   /* enough for the first 10 000 terms (empirical) */

/* --------------------------------------------------------------------- */
/*  Global data structures                                               */
/* --------------------------------------------------------------------- */

static uint32_t *g_primes       = NULL;   /* list of primes < = MAXK   */
static size_t   g_prime_cnt     = 0;      /* number of primes           */
static uint32_t *g_spf          = NULL;   /* smallest prime factor      */
static uint32_t *g_prime_index  = NULL;   /* prime -> index in g_primes  */
static size_t   g_parity_words  = 0;      /* words per bitset           */

static uint64_t **g_factParity = NULL;   /* parity of i! for 0..MAXK    */

static struct SeqState {
    size_t   term_count;   /* number of terms found          */
    seq_t   *terms;        /* array of found k values        */
    size_t   term_cap;     /* allocated capacity              */
    size_t   cur_k;        /* current maximum k processed    */
    uint64_t *superParity; /* XOR of parity vectors of 1..k! */
    bool     init_done;    /* has initialization been done?  */
} g_state = {0};

/* --------------------------------------------------------------------- */
/*  Bitset utilities                                                    */
/* --------------------------------------------------------------------- */

static inline void toggle_prime_bit(uint64_t *bits, uint32_t prime)
{
    uint32_t idx = g_prime_index[prime];
    if (idx == (uint32_t)-1) return;          /* safety: prime not found */
    bits[idx >> 6] ^= 1ULL << (idx & 63);
}

/* --------------------------------------------------------------------- */
/*  Prime sieve and factorisation tables                                */
/* --------------------------------------------------------------------- */

static void init_primes(void)
{
    uint8_t  *is_composite = (uint8_t*)calloc(MAXK + 1, 1);
    g_spf            = (uint32_t*)calloc(MAXK + 1, sizeof(uint32_t));
    g_prime_index    = (uint32_t*)calloc(MAXK + 1, sizeof(uint32_t));

    /* Initially allocate more than needed for the prime list */
    g_primes = (uint32_t*)malloc(((MAXK / 10) + 1) * sizeof(uint32_t));

    for (uint32_t i = 2; i <= MAXK; ++i) {
        if (!is_composite[i]) {                      /* i is prime */
            g_prime_index[i] = (uint32_t)g_prime_cnt;
            if (g_prime_cnt == MAXK / 10) {           /* grow array */
                g_primes = (uint32_t*)realloc(g_primes,
                    ((g_prime_cnt / 10) + 1) * sizeof(uint32_t));
            }
            g_primes[g_prime_cnt++] = i;
            g_spf[i] = i;
        } else {
            g_prime_index[i] = (uint32_t)-1;
        }

        for (size_t j = 0; j < g_prime_cnt &&
                         g_primes[j] <= g_spf[i] &&
                         (uint64_t)i * g_primes[j] <= MAXK; ++j) {
            uint32_t v = (uint32_t)(i * g_primes[j]);
            is_composite[v] = 1;
            if (!g_spf[v]) g_spf[v] = g_primes[j];
        }
    }

    free(is_composite);
    g_parity_words = (g_prime_cnt + 63) / 64;
}

/* --------------------------------------------------------------------- */
/*  Pre‑compute parity vectors of factorials 1! … MAXK!                 */
/* --------------------------------------------------------------------- */

static void init_factParity(void)
{
    g_factParity = (uint64_t**)calloc(MAXK + 1, sizeof(uint64_t*));
    for (size_t i = 0; i <= MAXK; ++i) {
        g_factParity[i] = (uint64_t*)calloc(g_parity_words, sizeof(uint64_t));
    }
    /* parity of 0! = 1, i.e. zero parity vector already */
    for (size_t m = 1; m <= MAXK; ++m) {
        /* Start with parity of (m-1)! */
        bitset_copy(g_factParity[m], g_factParity[m - 1], g_parity_words);

        /* Toggle bits for the prime factors of m, based on parity of exponents */
        uint32_t x = (uint32_t)m;
        while (x > 1) {
            uint32_t p = g_spf[x];
            unsigned exp_parity = 0;
            while (x % p == 0) {
                x /= p;
                exp_parity ^= 1;
            }
            if (exp_parity) toggle_prime_bit(g_factParity[m], p);
        }
    }
}

/* --------------------------------------------------------------------- */
/*  One‑time initialization                                              */
/* --------------------------------------------------------------------- */

static void seq_init(void)
{
    if (g_state.init_done) return;
    init_primes();
    init_factParity();

    g_state.term_cap = 1000;
    g_state.terms = (seq_t*)calloc(g_state.term_cap, sizeof(seq_t));
    g_state.superParity = (uint64_t*)calloc(g_parity_words, sizeof(uint64_t));

    g_state.cur_k = 0;
    g_state.term_count = 0;
    g_state.init_done = true;
}

/* --------------------------------------------------------------------- */
/*  Test whether current k is a term                                   */
/* --------------------------------------------------------------------- */

static bool is_term(size_t k)
{
    if (k == 1) return true;                 /* 1 is the first term */
    if (k & 1) return false;                 /* odd k>1 cannot be a term */

    size_t mid = k >> 1;

    for (int dm = -2; dm <= 2; ++dm) {
        size_t m = (size_t)(mid + dm);
        if (m < 1 || m > k) continue;
        if (bitset_equal(g_state.superParity, g_factParity[m], g_parity_words))
            return true;
    }
    return false;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                               */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* out of range */

    seq_init();

    /* Return already computed term if available */
    if ((size_t)n <= g_state.term_count)
        return g_state.terms[n - 1];

    /* Generate terms up to the requested index */
    while ((size_t)n > g_state.term_count) {
        ++g_state.cur_k;   /* next k */

        /* Update superParity ^= parity of k! */
        for (size_t w = 0; w < g_parity_words; ++w)
            g_state.superParity[w] ^= g_factParity[g_state.cur_k][w];

        if (is_term(g_state.cur_k)) {
            if (g_state.term_count == g_state.term_cap) {
                g_state.term_cap *= 2;
                g_state.terms = (seq_t*)realloc(g_state.terms,
                                                g_state.term_cap * sizeof(seq_t));
            }
            g_state.terms[g_state.term_count++] = (seq_t)g_state.cur_k;
        }
    }

    return g_state.terms[n - 1];
}
