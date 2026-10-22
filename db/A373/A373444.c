/*
 * A373444  Positive integers that cannot be written as a sum of a practical
 *          number and a heptagonal number.
 * https://oeis.org/A373444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 72, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A373444: positive integers that cannot be written as a sum of a
   practical number and a heptagonal number. The function builds all such
   numbers up to a fixed limit and returns the n‑th term (1‑based). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Fixed limit that safely contains the first 10 000 terms (the largest such
   integer conjecturally is 79445 < 2·10⁶). */
#define LIMIT 2000000U

/* ----------------------------------------------------------------------- */
/* Utility: smallest prime factor sieve for numbers up to LIMIT.            */
static uint32_t *build_spf(void){
    uint32_t *spf = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!spf) return NULL;
    memset(spf, 0, (LIMIT + 1) * sizeof(uint32_t));
    for (uint32_t i = 2; i <= LIMIT; ++i){
        if (spf[i] == 0){                /* i is prime */
            spf[i] = i;
            if ((uint64_t)i * i <= LIMIT){
                for (uint32_t j = i * i; j <= LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    return spf;
}

/* ----------------------------------------------------------------------- */
/* Test whether x is a practical number using the spf table.                 */
static bool is_practical(uint32_t x, const uint32_t *spf){
    if (x == 1) return true;
    uint64_t sigma = 1;          /* sum of divisors of the partial product */
    while (x > 1){
        uint32_t p = spf[x];
        uint32_t exp = 0;
        while (x % p == 0){      /* factor out p */
            x /= p;
            ++exp;
        }
        /* p_i must satisfy p <= sigma + 1   (p = 2 is always allowed) */
        if (p != 2 && p > sigma + 1) return false;
        /* multiply sigma by sum of powers of p (p^(exp+1)-1)/(p-1) */
        uint64_t powp = 1;
        for (uint32_t i = 0; i < exp + 1; ++i) powp *= p;
        uint64_t sigma_factor = (powp - 1) / (p - 1);
        sigma *= sigma_factor;
    }
    return true;
}

/* ----------------------------------------------------------------------- */
/* Build the sequence and cache it in static storage.                        */
static void build_cache(seq_t **seq_ptr, size_t *seq_len_ptr){
    static bool initialized = false;
    if (initialized) return;

    /* 1. smallest prime factor table */
    uint32_t *spf = build_spf();
    if (!spf) return;

    /* 2. collect all practical numbers up to LIMIT */
    uint32_t *practical = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    size_t practical_cnt = 0;
    for (uint32_t i = 1; i <= LIMIT; ++i)
        if (is_practical(i, spf)) practical[practical_cnt++] = i;

    /* 3. list all heptagonal numbers (include 0) up to LIMIT */
    uint32_t *hept = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    size_t hept_cnt = 0;
    for (uint32_t m = 0;; ++m){
        uint64_t h = (uint64_t)m * (5U * m - 3U) / 2U;
        if (h > LIMIT) break;
        hept[hept_cnt++] = (uint32_t)h;
    }

    /* 4. mark representable sums */
    bool *rep = (bool*)calloc(LIMIT + 1, sizeof(bool));
    for (size_t pi = 0; pi < practical_cnt; ++pi){
        uint32_t p = practical[pi];
        for (size_t hi = 0; hi < hept_cnt; ++hi){
            uint64_t sum = (uint64_t)p + hept[hi];
            if (sum > LIMIT) break;
            rep[(size_t)sum] = true;
        }
    }

    /* 5. build the sequence of non‑representable numbers */
    size_t cap = 128;
    seq_t *seq = (seq_t*)malloc(cap * sizeof(seq_t));
    size_t seq_len = 0;
    for (uint32_t k = 1; k <= LIMIT; ++k){
        if (!rep[(size_t)k]){
            if (seq_len == cap){
                cap <<= 1;
                seq = (seq_t*)realloc(seq, cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)k;
        }
    }

    /* 6. store results in static variables */
    *seq_ptr = seq;
    *seq_len_ptr = seq_len;

    /* 7. free temporary data */
    free(spf);
    free(practical);
    free(hept);
    free(rep);

    initialized = true;
}

/* ----------------------------------------------------------------------- */
/* Public API: return the n-th term (1‑based) of the sequence.               */
seq_t a(idx_t n){
    static seq_t *seq = NULL;
    static size_t seq_len = 0;

    if (n == 0) return 0;            /* offset handling */
    if (seq_len < (size_t)n) build_cache(&seq, &seq_len);
    return seq[n - 1];
}
