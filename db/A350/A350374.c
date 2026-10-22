/*
 * A350374  Numbers with exactly 7 semiprime divisors.
 * https://oeis.org/A350374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4175, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numbers with exactly seven semiprime divisors (OEIS A350374).
 *  These are the integers whose prime signature is {1,1,1,k} with k ≥ 2,
 *  i.e. numbers of the form p*q*r*s^k with p,q,r,s distinct primes
 *  and k ≥ 2.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#ifdef __cplusplus
#include <new>
#endif

/* External integer types defined by the build system */
extern idx_t _floor_isqrt_u64(idx_t n);
extern seq_t a(idx_t n);

/*====================================================================
 *  Internal state and helpers
 *====================================================================*/
static seq_t *seq_arr = NULL;      /* array of found sequence terms   */
static size_t seq_len = 0;         /* number of stored terms          */
static size_t seq_cap = 0;         /* current capacity of seq_arr     */
static uint64_t current_bound = 0; /* sieve bound used in last build  */

/* Ensure seq_arr has room for at least (need) elements */
static void seq_reserve(size_t need)
{
    if (seq_cap < need) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 128;
        while (new_cap < need) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_arr = tmp;
        seq_cap = new_cap;
    }
}

/* Append a value to the sequence, constructing it in place. */
static void seq_append(seq_t v)
{
    seq_reserve(seq_len + 1);
#ifdef __cplusplus
    new (&seq_arr[seq_len]) seq_t(v);   /* placement new for C++ */
#else
    seq_arr[seq_len] = v;              /* plain assignment for C */
#endif
    ++seq_len;
}

/* Release all elements of seq_arr (calls destructors when C++). */
static void seq_free_all(void)
{
#ifdef __cplusplus
    for (size_t i = 0; i < seq_len; ++i)
        seq_arr[i].~seq_t();
#endif
    free(seq_arr);
    seq_arr = NULL;
    seq_len = 0;
    seq_cap = 0;
}

/*====================================================================
 *  Sequence construction
 *====================================================================*/
/* Build the sequence up to 'bound' (inclusive).  All previous data
 * is discarded. */
static void build_sequence(uint64_t bound)
{
    seq_free_all();

    /* Small‑prime sieving using smallest prime factor (spf). */
    uint32_t *spf = (uint32_t *)malloc((size_t)(bound + 1) * sizeof(uint32_t));
    if (!spf) abort();
    spf[0] = 0;
    spf[1] = 1;
    uint32_t *primes = NULL;
    size_t primes_len = 0, primes_cap = 0;

    for (uint32_t i = 2; i <= bound; ++i) {
        if (!spf[i]) {            /* i is prime */
            spf[i] = i;
            if (primes_len == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 128;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!primes) abort();
            }
            primes[primes_len++] = i;
        }
        for (size_t j = 0; j < primes_len &&
             primes[j] <= spf[i] &&
             (uint64_t)i * primes[j] <= bound; ++j) {
            spf[(size_t)i * primes[j]] = primes[j];
        }
    }

    /* Collect numbers with prime signature {1,1,1,k}. */
    for (uint32_t i = 2; i <= bound; ++i) {
        uint32_t x = i;
        uint32_t distinct = 0;
        bool single_with_exp_ge_2 = false;
        while (x > 1) {
            uint32_t p = spf[x];
            uint32_t exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            ++distinct;
            if (exp >= 2) {
                if (single_with_exp_ge_2) { single_with_exp_ge_2 = false; break; }
                single_with_exp_ge_2 = true;
            }
        }
        if (distinct == 4 && single_with_exp_ge_2)
            seq_append((seq_t)i);
    }

    free(spf);
    free(primes);
    current_bound = bound;
}

/* Ensure that the sequence contains at least 'n' terms. */
static void ensure_build(idx_t n)
{
    if (seq_len >= (size_t)n) return;

    uint64_t bound = current_bound ? current_bound * 2 : 1000000ULL;
    while (seq_len < (size_t)n) {
        build_sequence(bound);
        if (seq_len >= (size_t)n) return;
        bound *= 2;
    }
}

/*====================================================================
 *  Public API
 *====================================================================*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* undefined for n == 0 */
    ensure_build(n);
    return seq_arr[n - 1];
}
