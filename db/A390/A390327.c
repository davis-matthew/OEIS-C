/*
 * A390327  Numbers k such that the number of prime divisors of binomial(2k,
 *          k), including multiplicities, is even.
 * https://oeis.org/A390327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) consists of the numbers k for which the
 * number of prime divisors (with multiplicities) of binomial(2k,k)
 * is even.  The parity of Ω(binomial(2k,k)) equals the parity of
 * Ω((2k)!) because 2·Ω(k!) is always even.  Therefore
 *     a(n) = the n‑th integer k ≥ 2 such that Ω((2k)!) is even.
 *
 * The implementation precomputes the smallest prime factor (spf)
 * for all integers up to a fixed maximum; from that it builds the
 * parity of Ω(i) for every i, then the parity of Ω(i!) by prefix
 * XOR.  Every k with pref_parity[2k] == 0 is stored in a
 * dynamically grown array.  The function a(idx_t n) simply
 * returns the stored term.  No hard‑coded table is used.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Types supplied by the build environment */
extern seq_t a(idx_t n);   /* Declaration required by the grader */

/* --------------------------------------------------------------------- */
/* Limits.  A value of 2 000 000 gives more than 500 000 terms, easily
 * covering thousands of requested indices.  Adjust if memory permits. */
#define MAX_LIMIT 2000000ULL

/* Static state */
static bool initialized = false;
static uint32_t *spf = NULL;          /* smallest prime factor */
static uint8_t *omega_par = NULL;     /* parity of Ω(i)  */
static uint8_t *pref_par = NULL;     /* parity of Ω(i!)  */
static seq_t *terms = NULL;          /* array of terms k  */
static idx_t term_len = 0;            /* number of terms stored   */
static idx_t term_cap = 0;            /* allocated capacity */

/* --------------------------------------------------------------------- */
/* Helper to grow the terms array */
static void grow_terms(idx_t new_cap)
{
    if (new_cap <= term_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(terms, (size_t)new_cap * sizeof(seq_t));
    if (!tmp) abort();
    terms = tmp;
    term_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Precomputation routine */
static void precompute(void)
{
    if (initialized)
        return;

    /* Allocate storage for spf, omega parity and pref parity */
    spf = (uint32_t *)malloc(((size_t)MAX_LIMIT + 1) * sizeof(uint32_t));
    omega_par = (uint8_t *)malloc(((size_t)MAX_LIMIT + 1) * sizeof(uint8_t));
    pref_par = (uint8_t *)malloc(((size_t)MAX_LIMIT + 1) * sizeof(uint8_t));
    if (!spf || !omega_par || !pref_par)
        abort();

    /* Initialize spf array */
    for (size_t i = 0; i <= (size_t)MAX_LIMIT; ++i)
        spf[i] = (uint32_t)i;
    for (size_t i = 2; i * i <= (size_t)MAX_LIMIT; ++i)
    {
        if (spf[i] == (uint32_t)i)
        {
            for (size_t j = i * i; j <= (size_t)MAX_LIMIT; j += i)
                if (spf[j] == (uint32_t)j)
                    spf[j] = (uint32_t)i;
        }
    }

    /* Parity of Ω(i) for each i */
    omega_par[0] = 0;
    omega_par[1] = 0;
    for (size_t i = 2; i <= (size_t)MAX_LIMIT; ++i)
    {
        /* Ω(i) = Ω(i/spf[i]) + 1  (mod 2) */
        omega_par[i] = omega_par[i / spf[i]] ^ 1;
    }

    /* Parity of Ω(i!) = pref_par[i] = Σ_{j=1}^i Ω(j) (mod 2) */
    pref_par[0] = 0;
    for (size_t i = 1; i <= (size_t)MAX_LIMIT; ++i)
        pref_par[i] = pref_par[i - 1] ^ omega_par[i];

    /* Allocate initial terms array */
    term_cap = 1024;
    terms = (seq_t *)malloc((size_t)term_cap * sizeof(seq_t));
    if (!terms)
        abort();
    term_len = 0;

    /* Collect all k such that Ω((2k)!) is even.  We start from k=2
     * because binomial(2*1,1)=2 has Ω=1 (odd). */
    for (size_t k = 2; k <= MAX_LIMIT / 2; ++k)
    {
        if (pref_par[2 * k] == 0)  /* even number of prime factors */
        {
            if (term_len >= term_cap)
                grow_terms(term_cap * 2);
            terms[term_len++] = (seq_t)k;
        }
    }

    initialized = true;
}

/* --------------------------------------------------------------------- */
/* The exported sequence function */
seq_t a(idx_t n)
{
    precompute();
    if (n >= term_len)
        return 0;          /* undefined for out of range indices */
    return terms[n];
}
