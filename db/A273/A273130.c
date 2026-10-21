/*
 * A273130  Numbers which have only positive entries in the difference table
 *          of their divisors.
 * https://oeis.org/A273130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose divisor difference table contains only strictly
   positive entries.  The function generates terms on demand, caching
   results and using a sieve of smallest prime factors to factor candidates
   efficiently.  The sieve is rebuilt only when the current candidate
   exceeds the current limit. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----  smallest prime factor sieve  --------------------------------- */

static uint32_t *spf = NULL;   /* spf[i] = smallest prime factor of i (i≥2) */
static size_t   spf_cap = 0;   /* largest index for which spf is valid */

/* rebuild spf up to 'limit' (inclusive) */
static void build_spf(size_t limit)
{
    if (limit <= spf_cap) return;          /* already sufficient */

    free(spf);                              /* discard old buffer */

    spf = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    if (!spf) exit(EXIT_FAILURE);

    if (limit >= 1) spf[1] = 1;
    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                  /* i is prime */
            spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    spf_cap = limit;
}

/* ----  test – difference table must be strictly positive  ---------- */

static bool has_positive_diff_table(seq_t n)
{
    /* factor n by spf: gather distinct prime powers */
    size_t tmp = (size_t)n;
    uint64_t primes[16];
    int       exps[16];
    int       pcnt = 0;

    while (tmp > 1) {
        uint32_t p = spf[tmp];
        int      e = 0;
        while (tmp % p == 0) { tmp /= p; ++e; }
        primes[pcnt] = p;
        exps[pcnt]   = e;
        ++pcnt;
    }

    /* generate all divisors of n (they fit in a small array) */
    seq_t divs[512];
    size_t dcnt = 1;
    divs[0] = 1;
    for (int i = 0; i < pcnt; ++i) {
        uint64_t p   = primes[i];
        int      e   = exps[i];
        size_t   old = dcnt;
        seq_t    ppow = 1;
        for (int k = 1; k <= e; ++k) {
            ppow *= (seq_t)p;
            for (size_t j = 0; j < old; ++j)
                divs[dcnt++] = divs[j] * ppow;
        }
    }

    /* sort divisor list (insertion sort – dcnt is tiny) */
    for (size_t i = 1; i < dcnt; ++i) {
        seq_t key = divs[i];
        size_t j  = i;
        while (j > 0 && divs[j - 1] > key) {
            divs[j] = divs[j - 1];
            --j;
        }
        divs[j] = key;
    }

    /* build successive difference rows, checking positivity */
    seq_t row[512];
    for (size_t i = 0; i < dcnt; ++i) row[i] = divs[i];
    size_t len = dcnt;

    seq_t tmp_row[512];
    while (len > 1) {
        for (size_t i = 0; i + 1 < len; ++i) {
            if (row[i + 1] <= row[i]) return false;   /* non‑positive */
            tmp_row[i] = row[i + 1] - row[i];
        }
        for (size_t i = 0; i < len - 1; ++i) row[i] = tmp_row[i];
        --len;
    }
    return true;
}

/* ----  sequence cache  --------------------------------------------- */

static seq_t *seq_cache = NULL;     /* array of found terms             */
static size_t seq_cap   = 0;        /* allocated capacity in entries    */
static size_t seq_len   = 0;        /* number of terms stored           */

static idx_t  next_candidate = 1;   /* next integer to test */

/* ----  sequence function  ------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for out‑of‑range input */

    if (seq_len < (size_t)n) {          /* generate terms if needed        */
        /* initial sieve limit */
        if (spf_cap == 0) build_spf(32768);

        while (seq_len < (size_t)n) {
            if ((size_t)next_candidate > spf_cap) {
                size_t new_cap = spf_cap ? spf_cap * 2 : 100000;
                if ((size_t)next_candidate > new_cap) new_cap = (size_t)next_candidate;
                build_spf(new_cap);
            }

            if (has_positive_diff_table(next_candidate)) {
                /* grow cache if necessary */
                if (seq_len == seq_cap) {
                    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                    seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    seq_cache = tmp;
                    seq_cap   = new_cap;
                }
                seq_cache[seq_len++] = next_candidate;
            }
            ++next_candidate;
        }
    }
    return seq_cache[n - 1];
}
