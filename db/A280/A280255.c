/*
 * A280255  Numbers k such that tau(k^(k+1)) is a prime.
 * https://oeis.org/A280255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 653, u32 n <= 653, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS sequence A280255: Numbers k such that tau(k^(k+1)) is a prime.
 * tau(k^(k+1)) = product over prime powers p^e dividing k of (e*(k+1)+1).
 * The product is prime only when k is a prime power and e*(k+1)+1 is prime.
 * We generate k in increasing order, test for prime‑power structure via
 * smallest prime factor sieve, compute e*(k+1)+1 and test primality.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_spf.h"      /* build_spf(idx_t limit, idx_t **spf_out) */
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t n) */

/* Maximum k we are willing to inspect; enough for n up to ~10⁴ */
static const idx_t MAX_K = 8000000;

static idx_t *spf = NULL;             /* smallest prime factor table */
static idx_t next_k = 2;              /* next k to test */
static seq_t terms[12000];             /* cached sequence values */
static size_t terms_len = 0;           /* number of cached terms */

/* Helper to initialise the SPF table once */
static void init_spf(void) {
    if (spf == NULL) {
        build_spf(MAX_K, &spf);
    }
}

/* Append a new term to the cache */
static void add_term(idx_t k) {
    if (terms_len >= sizeof(terms) / sizeof(terms[0]))
        return;                       /* too many terms – should not happen */
    terms[terms_len++] = (seq_t)k;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n <= 0) return 0;              /* undefined for n ≤ 0 */

    init_spf();

    /* Generate terms until we have at least n terms */
    while (terms_len < (size_t)n) {
        for (; next_k <= MAX_K && terms_len < (size_t)n; ++next_k) {
            /* Factor k using SPF to check if it is a prime power */
            idx_t cur = next_k;
            if (cur < 2) continue;
            idx_t first_pf = spf[cur];
            if (first_pf == 0) continue;  /* should not happen */

            size_t distinct = 0;
            idx_t exp = 0;

            while (cur > 1 && distinct < 2) {
                idx_t p = spf[cur];
                idx_t cnt = 0;
                while (cur % p == 0) {
                    cur /= p;
                    ++cnt;
                }
                if (distinct == 0) exp = cnt;
                ++distinct;
            }
            if (distinct != 1) continue;  /* not a prime power */

            /* Compute e*(k+1)+1 and test primality */
            seq_t val = (seq_t)exp * (seq_t)(next_k + 1) + 1;
            if (is_prime_big(val)) {
                add_term(next_k);
            }
        }
        if (terms_len < (size_t)n) {
            /* We ran out of precomputed SPF range – shouldn't happen for n≤10⁴ */
            break;
        }
    }

    return terms[n - 1];
}
