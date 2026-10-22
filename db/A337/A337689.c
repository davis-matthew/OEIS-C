/*
 * A337689  Numbers divisible by 3 or more primitive nondeficient numbers.
 * https://oeis.org/A337689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1282, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A337689: numbers divisible by at least three primitive nondeficient
   numbers – sieve‑based implementation (computes all terms up to a fixed
   limit once, then indexes them). */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"

/* static storage for the generated sequence */
static seq_t *SEQ = NULL;
static size_t SEQ_SIZE = 0;

/* ------------------------------------------------------------ */
/* Build the sequence up to LIMIT.  The function is idempotent. */
/* ------------------------------------------------------------ */
static void init_sequence(void)
{
    if (SEQ != NULL) return;          /* already built */

    /* Ensure we have enough terms (≥ 10 000) by choosing a sufficiently
       large upper bound.  Every multiple of 140 appears in the sequence,
       so LIMIT = 3 200 000 yields ≈ 22 857 terms. */
    const idx_t LIMIT = 3200000;
    const size_t N = (size_t)LIMIT;   /* array sizes */

    /* ----------- linear sieve for smallest prime factors ------------ */
    uint32_t *spf      = (uint32_t*)calloc(N + 1, sizeof(uint32_t));
    uint32_t *primes   = (uint32_t*)malloc((N + 1) * sizeof(uint32_t));
    size_t pc = 0;
    for (size_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            primes[pc++] = (uint32_t)i;
        }
        for (size_t j = 0; j < pc; ++j) {
            uint32_t p = primes[j];
            uint64_t v = (uint64_t)p * i;
            if (v > N) break;
            spf[(size_t)v] = p;
            if (p == spf[i]) break;
        }
    }

    /* --------- compute sum of divisors sigma(n) for all n ----------- */
    uint64_t *sigma = (uint64_t*)calloc(N + 1, sizeof(uint64_t));
    sigma[1] = 1;
    for (size_t i = 2; i <= N; ++i) {
        uint32_t p = spf[i];
        size_t temp = i;
        size_t e = 0;
        while (temp % p == 0) { temp /= p; ++e; }

        /* sigma of the remaining co-prime part */
        uint64_t sigma_m = sigma[temp];
        /* geometric series sum for p^e */
        uint64_t pp = 1;
        for (size_t k = 0; k < e + 1; ++k) pp *= (uint64_t)p;
        uint64_t sigma_p = (pp - 1) / ((uint64_t)p - 1);

        sigma[i] = sigma_m * sigma_p;
    }

    /* ---------- identify nondeficient numbers --------------------- */
    uint8_t *is_nondef = (uint8_t*)calloc(N + 1, sizeof(uint8_t));
    for (size_t i = 2; i <= N; ++i) {
        if (sigma[i] >= ((seq_t)2 * (seq_t)i))
            is_nondef[i] = 1;
    }

    /* ---------- minimal proper nondeficient divisor for each n ----- */
    uint32_t *min_nondef = (uint32_t*)calloc(N + 1, sizeof(uint32_t));
    for (size_t i = 2; i <= N; ++i) {
        if (is_nondef[i]) {
            for (uint32_t k = (uint32_t)(i + i); k <= N; k += (uint32_t)i)
                if (min_nondef[k] == 0) min_nondef[k] = (uint32_t)i;
        }
    }

    /* --------- collect primitive nondeficient numbers (PDN) --------- */
    uint32_t *pdns = (uint32_t*)malloc((N + 1) * sizeof(uint32_t));
    size_t pdn_cnt = 0;
    for (size_t i = 2; i <= N; ++i) {
        if (is_nondef[i] && min_nondef[i] == 0)
            pdns[pdn_cnt++] = (uint32_t)i;
    }

    /* --------- count PDN divisors for each n ----------------------- */
    uint16_t *cnt = (uint16_t*)calloc(N + 1, sizeof(uint16_t));
    for (size_t idx = 0; idx < pdn_cnt; ++idx) {
        uint32_t d = pdns[idx];
        for (uint64_t k = d; k <= N; k += d)
            if (cnt[k] < UINT16_MAX) ++cnt[k];
    }

    /* ---------- build the final sequence array --------------------- */
    size_t seq_len = 0;
    seq_t *seq_arr = seq_alloc(N + 5000);   /* sufficiently large */
    for (size_t i = 1; i <= N; ++i) {
        if (cnt[i] >= 3) seq_arr[seq_len++] = (seq_t)i;
    }

    SEQ = seq_arr;
    SEQ_SIZE = seq_len;

    /* free temporary data */
    free(spf);
    free(primes);
    free(sigma);
    free(is_nondef);
    free(min_nondef);
    free(cnt);
    free(pdns);
}

/* ------------------------------------------------------------ */
/*  Return the nth term of A337689 (1‑based indexing).          */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_sequence();
    if (n == 0 || n > SEQ_SIZE) return 0;  /* out of range – undefined  */
    return SEQ[n - 1];
}
