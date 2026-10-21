/*
 * A292632  a(n) = n! * [x^n] exp((n+2)*x)*(BesselI(0,2*x) - BesselI(1,2*x)).
 * https://oeis.org/A292632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binomial(n,k) * Catalan(k) * n^(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/binomial.h"
#include "../utils/catalan.h"
#include "../utils/big_pow.h"

/* Forward declarations for internal allocation helpers */
static seq_t* alloc_seq(size_t n);
static seq_t* realloc_seq(seq_t* ptr, size_t oldn, size_t newn);

/* --------------------------------------------------------------------------
   Simple re‑allocation helpers that work both for C and C++.
   -------------------------------------------------------------------------- */
static seq_t* alloc_seq(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t*)malloc(n * sizeof(seq_t));
#endif
}

static seq_t* realloc_seq(seq_t* ptr, size_t oldn, size_t newn)
{
    if (ptr == NULL)
        return alloc_seq(newn);
#ifdef __cplusplus
    seq_t* newptr = new seq_t[newn];
    for (size_t i = 0; i < oldn; ++i)
        newptr[i] = ptr[i];
    delete[] ptr;
    return newptr;
#else
    return (seq_t*)realloc(ptr, newn * sizeof(seq_t));
#endif
}

/* --------------------------------------------------------------------------
   Cache for the sequence values.
   -------------------------------------------------------------------------- */
static seq_t* a_cache = NULL;
static size_t a_cache_size = 0;   /* number of stored terms */

/* Fill the cache up to and including n. */
static void ensure_a(idx_t n)
{
    if ((size_t)n < a_cache_size)
        return;

    size_t new_size = (size_t)n + 1;
    a_cache = realloc_seq(a_cache, a_cache_size, new_size);

    for (size_t i = a_cache_size; i < new_size; ++i) {
        idx_t ii = (idx_t)i;
        seq_t n_val = (seq_t)ii;

        /* n^n once */
        seq_t n_pow = big_pow(n_val, (seq_t)ii);

        seq_t binom = (seq_t)1;          /* binomial(ii,0)   */
        seq_t sum   = (seq_t)0;

        for (idx_t k = 0; k <= ii; ++k) {
            seq_t term   = binom;
            term *= catalan(k);
            term *= n_pow;
            sum += term;

            if (k < ii) {
                /* binomial(ii,k+1) = binomial(ii,k)*(ii-k)/(k+1) */
                seq_t num = binom * (seq_t)(ii - k);
                binom      = num / (seq_t)(k + 1);

                /* n_pow = n^(ii-k-1) */
                n_pow /= n_val;
            }
        }
        a_cache[(size_t)i] = sum;
    }

    a_cache_size = new_size;
}

/* --------------------------------------------------------------------------
   Public entry point expected by the build system.
   -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_a(n);
    return a_cache[(size_t)n];
}
