/*
 * A339462  Numbers k such that (i*k)^(1/3) is an integer for some i in 1 <= i
 *          <= k.
 * https://oeis.org/A339462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer k such that there exists an integer i, * 1 ≤
 *            i ≤ k, with (i*k)^(1/3) an integer. The function generates the *
 *            terms lazily by testing successive k and storing those that
 *            satisfy * the condition. It keeps a small sieve of smallest
 *            prime factors * to check the condition efficiently
 * Range:     u8 n <= 46, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static state ----------------------------------------------------- */
static uint32_t *spf       = NULL;   /* smallest prime factor array          */
static size_t   spf_size   = 0;     /* size of the spf array                 */

static seq_t   *terms      = NULL;   /* dynamic array of sequence values      */
static size_t   term_cap   = 0;      /* current capacity                       */
static size_t   term_cnt   = 0;      /* number of stored terms                 */
static idx_t    last_k     = 0;      /* largest k that has been checked        */

/* ------------------------------------------------------------------ */
/* Allocate or enlarge the spf sieve up to 'up_to' (inclusive).      */
static void ensure_spf(size_t up_to)
{
    if (up_to <= spf_size) return;

    size_t new_size = spf_size ? spf_size : 256;
    while (new_size < up_to) new_size <<= 1;

    uint32_t *new_spf = (uint32_t *)calloc(new_size + 1, sizeof(uint32_t));
    if (!new_spf) abort();

    for (size_t i = 2; i <= new_size; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = (uint32_t)i;
            if (i <= new_size / i) {
                for (size_t j = i * i; j <= new_size; j += i)
                    if (new_spf[j] == 0) new_spf[j] = (uint32_t)i;
            }
        }
    }
    free(spf);
    spf = new_spf;
    spf_size = new_size;
}

/* ------------------------------------------------------------------ */
/* Test whether a particular k satisfies the condition.  Return true
 * if there exists an integer i <= k such that i*k is a perfect cube. */
static bool is_valid_k(idx_t k)
{
    idx_t m = 1;                 /* minimal multiplier to make k a cube   */
    idx_t x = k;

    while (x > 1) {
        uint32_t p   = spf[x];
        uint32_t exp = 0;
        while (x % p == 0) {
            x /= p;
            ++exp;
        }
        uint32_t need = (3 - exp % 3) % 3;   /* how many times p is needed */
        for (uint32_t i = 0; i < need; ++i) {
            if (m > k / p) return false;   /* would exceed k          */
            m *= p;
        }
    }
    return m <= k;
}

/* ------------------------------------------------------------------ */
/* Append a new term to the dynamic array.  The array is grown
 * geometrically if necessary using realloc. */
static inline void add_term(seq_t v)
{
    if (term_cnt == term_cap) {
        size_t new_cap = term_cap ? term_cap << 1 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
        term_cap = new_cap;
    }
    terms[term_cnt++] = v;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.  It will generate terms up to the requested
 * index if they have not yet been computed. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n==0, but harmless */

    /* initialise storage lazily */
    if (!terms) {
        terms = (seq_t *)malloc(16 * sizeof(seq_t));
        if (!terms) abort();
        term_cap = 16;
        term_cnt = 0;
    }

    while (term_cnt < (size_t)n) {
        ++last_k;
        ensure_spf((size_t)last_k);
        if (is_valid_k(last_k))
            add_term((seq_t)last_k);
    }

    return terms[n - 1];
}
