/*
 * A373257  Numbers k for which A003415(k) == -1 (mod 3), where A003415 is the
 *          arithmetic derivative.
 * https://oeis.org/A373257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A373257:
   Numbers k for which the arithmetic derivative D(k) ≡ -1 (mod 3). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"

/* External types provided by the build system */
 /* seq_t and idx_t are defined elsewhere */

/* ---------- Helper data structures ---------- */
static uint64_t *spf      = NULL;        /* smallest prime factor array */
static size_t   spf_len   = 0;           /* actual filled length   */
static size_t   spf_cap   = 0;           /* allocated capacity     */

static uint64_t *primes   = NULL;        /* list of primes found   */
static size_t   primes_len = 0;
static size_t   primes_cap = 0;

/* Collected terms satisfying D(k) ≡ 2 (mod 3)            */
static seq_t   *terms    = NULL;
static size_t   terms_len = 0;
static size_t   terms_cap = 0;

/* last integer for which D has been examined            */
static size_t processed = 1;

/* ---------- Utility: grow a linear sieve (spf) ---------- */
static void extend_spf_to (size_t new_limit)
{
    if (new_limit + 1 <= spf_len) return;      /* already long enough */

    /* make sure we have room for new_limit + 1 entries          */
    ensure_capacity(new_limit + 1, &spf_cap, (void **)&spf, sizeof(uint64_t));

    /* initialise new part with 0 (unknown smallest prime factor)*/
    if (spf_len == 0) {                          /* first time initialisation   */
        spf[0] = 0;
        spf[1] = 1;
        spf_len = 2;
    }
    for (size_t i = spf_len; i <= new_limit; ++i)
        spf[i] = 0;

    /* linear sieve : fill spf and primes in the new range      */
    for (size_t i = spf_len; i <= new_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            /* grow primes array if needed */
            if (primes_len == primes_cap)
                ensure_capacity(primes_len + 1, &primes_cap, (void **)&primes,
                                sizeof(uint64_t));
            primes[primes_len++] = i;
        }
        for (size_t j = 0; j < primes_len; ++j) {
            uint64_t p = primes[j];
            if (p > spf[i] || i > new_limit / p)
                break;
            size_t prod = i * p;
            if (prod > new_limit)
                break;
            spf[prod] = p;
        }
    }

    spf_len = new_limit + 1;
}

/* ---------- Compute D(k) mod 3 using the factorisation --------- */
static uint8_t d_mod3 (uint64_t k)
{
    if (k <= 1) return 0;

    uint8_t sum = 0;
    uint64_t temp = k;
    seq_t  n_val = (seq_t)k;          /* keep a seq_t copy for division */

    while (temp > 1) {
        uint64_t p = spf[temp];
        uint64_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        seq_t np = n_val / (seq_t)p;        /* n/p as seq_t */
        uint8_t term = (uint8_t)((e % 3) *
                                 ((uint8_t)(np % 3)) % 3);
        sum += term;
        if (sum >= 3) sum -= 3;
    }
    return sum % 3;
}

/* ---------- Public sequence function ---------- */
seq_t a (idx_t n)
{
    if (n == 0) return 0;                /* undefined, but harmless */

    /* extend sieve and search until we have enough terms */
    while (terms_len < (size_t)n) {
        /* choose a new limit large enough to be efficient      */
        size_t target = processed + 1000;
        if (target < 1000) target = 1000;

        extend_spf_to(target);

        for (size_t k = processed + 1; k <= target; ++k) {
            if (d_mod3((uint64_t)k) == 2) {
                /* append to the terms array */
                if (terms_len == terms_cap)
                    ensure_capacity(terms_len + 1, &terms_cap,
                                    (void **)&terms, sizeof(seq_t));
                terms[terms_len++] = (seq_t)k;
            }
        }
        processed = target;
    }

    return terms[(size_t)n - 1];
}
