/*
 * A341274  Composite numbers k that are divisible by (k mod
 *          sopfr(k))+floor(k/sopfr(k)), where sopfr = A001414.
 * https://oeis.org/A341274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th composite k such that k is divisible by (k mod
 *            sopfr(k)) + floor(k / sopfr(k)), where sopfr(k) = A001414(k)
 *            (sum of prime factors with multiplicity)
 * Range:     u8 n <= 47, u16 n <= 1457, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a dynamic list of found terms and expands
   a sieve for smallest prime factors as needed.  It works for any
   seq_t/idx_t combination supported by the build.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"

/* ---------- Prime sieve for smallest prime factors ---------- */
static uint64_t *spf = NULL;   /* spf[i] = smallest prime divisor of i (0 for i < 2) */
static size_t   spf_cap = 0;   /* highest index initialized in spf */

/* Ensure spf covers indices up to at least 'limit' (inclusive). */
static void build_spf(size_t limit)
{
    if (limit <= spf_cap) return;

    uint64_t *new_spf = (uint64_t*)calloc(limit + 1, sizeof(uint64_t));
    if (!new_spf) exit(EXIT_FAILURE);

    for (size_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = i;
            if (i <= limit / i) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0) new_spf[j] = i;
            }
        }
    }

    free(spf);
    spf = new_spf;
    spf_cap = limit;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* safety fallback for undefined offset */

    /* Static state for incremental construction */
    static idx_t         computed_n   = 0;           /* number of terms already found */
    static size_t        terms_cap    = 0;           /* allocated capacity of terms */
    static seq_t *       terms        = NULL;        /* array of found terms */
    static idx_t         last_checked = 3;           /* last integer examined (>=3) */

    /* Ensure the sieve is large enough for upcoming candidates */
    if (last_checked + 1 > spf_cap)
        build_spf((last_checked + 1) * 2);

    /* Generate terms until we have at least 'n' of them */
    while (computed_n < n) {
        /* Find the next composite candidate */
        for (++last_checked; ; ++last_checked) {
            if (last_checked < 4) continue;                 /* 1,2,3 are not composite */

            if (last_checked > spf_cap)                    /* enlarge sieve if needed */
                build_spf(last_checked * 2);

            if (spf[last_checked] == last_checked) continue;/* prime – skip */

            /* Compute sopfr(last_checked) : sum of prime factors (with multiplicity) */
            uint64_t tmp  = last_checked;
            seq_t    sum = 0;
            while (tmp > 1) {
                uint64_t p = spf[tmp];
                sum += (seq_t)p;
                tmp /= p;
            }

            /* Compute divisor = floor(k / sum) + (k mod sum) */
            seq_t k = (seq_t)last_checked;
            seq_t t = k / sum;
            seq_t r = k % sum;
            seq_t divisor = t + r;

            if (divisor != 0 && (k % divisor == 0)) {
                /* Record this term */
                ++computed_n;

                if (computed_n > terms_cap) {              /* grow the array */
                    size_t new_cap = terms_cap ? terms_cap * 2 : 128;
                    terms = seq_realloc(terms, terms_cap, new_cap);
                    if (!terms) exit(EXIT_FAILURE);
                    terms_cap = new_cap;
                }
                terms[computed_n - 1] = k;

                if (computed_n == n) return k; /* desired term found */
                break; /* proceed to find the next term */
            }
        }
    }

    return terms[n - 1];   /* safety return – should not be reached */
}
