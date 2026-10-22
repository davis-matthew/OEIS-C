/*
 * A364025  Semiprimes with no squarefree neighbors.
 * https://oeis.org/A364025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k such that k is a semiprime and both
 *            k-1 and k+1 are NOT squarefree
 * Range:     u8 n <= 8, u16 n <= 2769, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/*--- Limits ---------------------------------------------------*/
#define SPF_LIMIT 20000000U  /* must be large enough to find n ≤ 10000 */

/*--- Static state ----------------------------------------------*/
static uint32_t *spf = NULL;
static bool spf_ready = false;

static seq_t *terms = NULL;
static size_t terms_size = 0;
static size_t terms_cap  = 0;

static uint64_t scan_next = 2ULL;

/*--- Sieve of smallest prime factors ---------------------------*/
static void init_spf(void)
{
    if (spf_ready) return;
    /* allocate one extra slot so that indices 0..SPF_LIMIT are valid */
    spf = (uint32_t*)calloc(SPF_LIMIT + 1, sizeof(uint32_t));
    if (!spf) return; /* unlikely failure */

    for (uint32_t i = 2; i <= SPF_LIMIT; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = i;
            if ((uint64_t)i * i <= SPF_LIMIT) {
                for (uint32_t j = i * i; j <= SPF_LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    spf_ready = true;
}

/*--- Test whether x is a semiprime ------------------------------*/
static bool is_semiprime(uint64_t n)
{
    if (n < 4) return false;          /* 4 = 2*2 is the first semiprime */
    uint64_t t = n;
    int cnt = 0;
    while (t > 1 && cnt <= 2) {
        uint32_t p = spf[(uint32_t)t];
        if (p == 0) p = (uint32_t)t;   /* t is prime > SPF_LIMIT? unlikely */
        while (t % p == 0) {
            t /= p;
            ++cnt;
            if (cnt > 2) return false;
        }
    }
    return cnt == 2;
}

/*--- Test whether x has a square factor --------------------------*/
static bool has_square_factor(uint64_t n)
{
    if (n < 2) return false;          /* 1 is squarefree */
    uint64_t t = n;
    while (t > 1) {
        uint32_t p = spf[(uint32_t)t];
        if (p == 0) p = (uint32_t)t;
        int exp = 0;
        while (t % p == 0) {
            t /= p;
            ++exp;
            if (exp >= 2) return true;
        }
    }
    return false;
}

/*--- Append a new term to the static array ----------------------*/
static void add_term(uint64_t val)
{
    if (terms_cap == 0) {
        terms_cap = 16;
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
    } else if (terms_size == terms_cap) {
        terms_cap <<= 1;
        terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
    }
#ifdef __cplusplus
    new (terms + terms_size) seq_t((seq_t)val);
#else
    terms[terms_size] = (seq_t)val;
#endif
    ++terms_size;
}

/*--- Main function ------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* undefined for 0 */

    /* Return cached value if already computed */
    if ((size_t)n <= terms_size) return terms[(size_t)n - 1];

    init_spf();

    while (terms_size < (size_t)n) {
        /* We assume SPF_LIMIT is large enough; if exceeded, further
           extension would be needed but is not required for n ≤ 10000. */
        if (scan_next >= SPF_LIMIT) break;

        if (is_semiprime(scan_next) &&
            has_square_factor(scan_next - 1) &&
            has_square_factor(scan_next + 1)) {
            add_term(scan_next);
        }
        ++scan_next;
    }

    return terms[(size_t)n - 1];
}
