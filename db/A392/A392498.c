/*
 * A392498  Numbers k > 1 with the smallest prime divisor p, where the sum of
 *          divisors d in the range 1 < d < k/p is less than k/p.
 * https://oeis.org/A392498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate sparse numbers by incremental search.
   For each candidate k > 1, we find its smallest prime divisor p
   and compute L = k / p.  If L == 1 (k is prime) we skip k.
   We enumerate all divisors of k, sum the divisors d with 1 < d < L,
   and if the sum is strictly less than L, we record k as the next
   term of the sequence.  The resulting list of terms is memoised
   so that any value of n can be answered in O(1) after the first few
   terms have been generated.  The implementation uses a dynamic
   array of smallest‑prime factors which is expanded as needed. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_spf.h"   /* provides void build_spf(idx_t limit, idx_t **spf_out) */

static idx_t *spf = NULL;          /* array of smallest prime factors, indexed by value */
static size_t spf_len = 0;         /* length of the spf array (valid indices: 0 .. spf_len-1) */

static seq_t *terms = NULL;        /* collected sparse numbers */
static size_t terms_len = 0;       /* number of terms stored */
static size_t terms_cap = 0;       /* current capacity of terms array */

static seq_t next_candidate = 4;   /* next k to test */

/* Ensure that the spf array covers indices up to 'limit' (inclusive). */
static void ensure_spf(size_t limit)
{
    if (limit < spf_len) return;

    size_t new_limit = spf_len > 0 ? spf_len * 2 : 4;
    if (new_limit <= limit) new_limit = limit + 1;   /* make sure we cover 'limit' itself */

    idx_t *new_spf = NULL;
    build_spf(new_limit, &new_spf);

    /* free old allocation if any */
    if (spf) free(spf);
    spf      = new_spf;
    /* build_spf is assumed to allocate an array of size new_limit+1 */
    spf_len  = new_limit + 1;
}

/* Append a value to the terms array, expanding it if necessary. */
static void append_term(seq_t val)
{
    if (terms_len >= terms_cap) {
        size_t new_cap = terms_cap == 0 ? 8 : terms_cap * 2;
        seq_t *new_terms = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (terms) { memcpy(new_terms, terms, terms_len * sizeof(seq_t)); free(terms); }
        terms = new_terms;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* Main sequence function.  Works for any build of seq_t. */
seq_t a(idx_t n)
{
    /* collect terms until we have at least n of them */
    while (terms_len < n) {
        /* make sure spf array covers the current candidate */
        ensure_spf((size_t)next_candidate);

        seq_t k = next_candidate;
        uint64_t p   = (uint64_t)spf[(size_t)k];   /* smallest prime factor of k */
        seq_t L = k / p;                           /* L = k / p */

        /* skip primes: L == 1 */
        if (L == 1) {
            next_candidate++;
            continue;
        }

        /* factorise k using spf */
        uint64_t primes[64];
        int exps[64];
        size_t pcnt = 0;
        seq_t temp = k;
        while (temp > 1) {
            uint64_t prime = (uint64_t)spf[(size_t)temp];
            int cnt = 0;
            while (temp % prime == 0) {
                temp /= prime;
                cnt++;
            }
            primes[pcnt] = prime;
            exps[pcnt]   = cnt;
            pcnt++;
        }

        /* generate all divisors of k */
        seq_t divs[2048];
        size_t dcnt = 1;
        divs[0] = 1;
        for (size_t i = 0; i < pcnt; i++) {
            uint64_t prime = primes[i];
            int exp = exps[i];
            size_t cur_dcnt = dcnt;
            seq_t pow = 1;
            for (int e = 1; e <= exp; e++) {
                pow *= prime;
                for (size_t j = 0; j < cur_dcnt; j++) {
                    divs[dcnt++] = divs[j] * pow;
                }
            }
        }

        /* compute the sum of admissible divisors */
        seq_t sum = 0;
        for (size_t i = 0; i < dcnt; i++) {
            seq_t d = divs[i];
            if (d > 1 && d < L) {
                sum += d;
                if (sum >= L) break;          /* early exit if sum already too large */
            }
        }

        /* if the condition holds, record the number */
        if (sum < L) append_term(k);

        next_candidate++;
    }

    return terms[n-1];
}
