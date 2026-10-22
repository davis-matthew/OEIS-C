/*
 * A359870  Numbers whose product of distinct prime factors is greater than
 *          the sum of its prime factors (with repetition).
 * https://oeis.org/A359870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 160, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A359870: numbers k where product of distinct prime
   factors > sum of prime factors with multiplicity.
   The algorithm performs a sieve for smallest prime factors up to a
   fixed upper bound and grows a table of discovered terms on demand. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Sieve for smallest prime factors -------------------------------- */
static unsigned int *spf = NULL;    /* array of smallest prime factors */
static idx_t spf_limit = 0;         /* allocated size */

/* Upper bound: large enough for n up to about 10 000. */
#define SIEVE_UPPER 5000000

static void build_spf(void)
{
    if (spf != NULL) return;          /* already built */

    spf_limit = SIEVE_UPPER + 1;
    spf = (unsigned int*)malloc(spf_limit * sizeof(unsigned int));
    if (spf == NULL) return;          /* allocation failure – unlikely */

    for (idx_t i = 0; i < spf_limit; ++i) spf[i] = 0;

    for (idx_t i = 2; i < spf_limit; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = (unsigned int)i;
            if ((idx_t)i * (idx_t)i < spf_limit) {
                for (idx_t j = i * i; j < spf_limit; j += i) {
                    if (spf[j] == 0) spf[j] = (unsigned int)i;
                }
            }
        }
    }
}

/* ---------- Test a candidate number ------------------------------------ */
static bool passes(uint32_t k)
{
    if (k == 1) return true;          /* 1 is a term */

    if (spf[k] == k) return false;    /* primes never satisfy */

    uint64_t rad = 1;                 /* product of distinct primes */
    uint64_t sum = 0;                 /* sum of prime factors (with multiplicity) */
    uint32_t temp = k;

    while (temp > 1) {
        uint32_t p = spf[temp];
        rad *= (uint64_t)p;
        do {
            sum += (uint64_t)p;
            temp /= p;
        } while (temp % p == 0);
    }

    return rad > sum;
}

/* ---------- Sequence storage ------------------------------------------- */
static seq_t  *terms = NULL;        /* array of found terms */
static idx_t   terms_len = 0;       /* number of terms stored */
static idx_t   terms_cap = 0;       /* capacity of the array */
static idx_t   next_k = 1;          /* next candidate integer to test */

static void grow_terms(void)
{
    terms_cap = (terms_cap == 0 ? 64 : terms_cap * 2);
    terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
}

/* ---------- Public sequence function --------------------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
        build_spf();
        terms_cap = 64;
        terms     = (seq_t*)malloc(terms_cap * sizeof(seq_t));
        terms_len = 0;
        next_k    = 1;
    }

    while (terms_len < n) {
        bool ok;
        if (next_k == 1) {
            ok = true;                    /* 1 is always a term */
        } else if (next_k < spf_limit && spf[next_k] == next_k) {
            ok = false;                   /* primes skip */
        } else {
            if (next_k >= spf_limit) {
                /* should not happen for the problem size; abort */
                ok = false;
            } else {
                ok = passes((uint32_t)next_k);
            }
        }

        if (ok) {
            if (terms_len == terms_cap) grow_terms();
            terms[terms_len++] = (seq_t)next_k;
        }

        ++next_k;
    }

    return terms[n - 1];
}
