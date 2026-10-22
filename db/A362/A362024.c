/*
 * A362024  The number of iterations of the infinitary totient function iphi
 *          (A064380) required to reach from n to 1.
 * https://oeis.org/A362024
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 762, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A362024
 *  a(n) = a( IPHI(n) ) + 1   (for n > 2)
 *  a(2) = 1
 */

#include <stdlib.h>     /* malloc, realloc, NULL */
#include <stdint.h>     /* uint64_t */
#include <stdbool.h>    /* bool */

static seq_t *a_cache        = NULL;   /* cache of a(n) */
static idx_t *phi_cache_int  = NULL;   /* cache of φ_inf(n), stored as idx_t */
static size_t cache_size     = 0;      /* allocated size of the caches    */

/* For the SPF sieve */
static idx_t *spf            = NULL;   /* smallest prime factor for each number */
static idx_t  spf_size       = 0;      /* largest index for which spf is defined (>=0) */

/* Ensure spf array is built up to at least n  */
static void ensure_spf(idx_t n)
{
    if (n <= spf_size)
        return;

    idx_t new_size = n + 1;             /* we need indices 0..n inclusive */
    spf = (idx_t*)realloc(spf, new_size * sizeof(idx_t));
    for (idx_t i = spf_size + 1; i < new_size; ++i)
        spf[i] = i;                     /* initially assume prime */

    for (idx_t i = 2; i * i < new_size; ++i)
        if (spf[i] == i)                /* i is prime */
            for (idx_t j = i * i; j < new_size; j += i)
                if (spf[j] == j)
                    spf[j] = i;

    spf_size = new_size - 1;            /* indices 0..new_size-1 valid now */
}

/* Ensure caches big enough for index n (inclusive) */
static void ensure_cache(idx_t n)
{
    size_t needed = n + 1;              /* to index n itself */
    if (needed <= cache_size)
        return;

    size_t new_size = cache_size ? cache_size << 1 : 32;
    while (new_size < needed)
        new_size <<= 1;

    a_cache = (seq_t*)realloc(a_cache, new_size * sizeof(seq_t));
    phi_cache_int = (idx_t*)realloc(phi_cache_int, new_size * sizeof(idx_t));

    for (size_t i = cache_size; i < new_size; ++i) {
        a_cache[i]       = 0;
        phi_cache_int[i] = 0;
    }
    cache_size = new_size;
}

/* Count m < n that are infinitarily coprime to n.
 *  Two numbers are infinitarily coprime iff for every prime p dividing n,
 *  the exponent of p in m and the exponent of p in n have no common
 *  binary 1‑bits:  (e_n & e_m) == 0.
 */
static seq_t compute_phi_inf(idx_t n)
{
    if (n == 2)                     /* φ_inf(2) = 1 by definition */
        return (seq_t)1;

    ensure_spf(n);

    /* factorise n: store primes and their exponents */
    idx_t temp = n;
    size_t np = 0;
    idx_t primes[20];   /* n <= 10000; 20 distinct primes is plenty */
    idx_t exps[20];
    while (temp > 1) {
        idx_t p = spf[temp];
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        primes[np] = p;
        exps[np]   = e;
        ++np;
    }

    /* Count eligible m */
    idx_t count = 0;
    for (idx_t m = 1; m < n; ++m) {
        bool ok = true;
        idx_t mm = m;
        for (size_t i = 0; i < np && ok; ++i) {
            idx_t e_m = 0;
            idx_t p = primes[i];
            while (mm % p == 0) {
                mm /= p;
                ++e_m;
            }
            if ((exps[i] & e_m) != 0)
                ok = false;
        }
        if (ok)
            ++count;
    }
    return (seq_t)count;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* undefined for n < 2, but not used */

    if (n == 2)
        return 1;                 /* a(2) = 1 by definition */

    ensure_cache(n);
    if (a_cache[n] != 0)
        return a_cache[n];

    idx_t phi;
    if (phi_cache_int[n] != 0)
        phi = phi_cache_int[n];
    else {
        phi = (idx_t)compute_phi_inf(n);
        phi_cache_int[n] = phi;
    }

    seq_t val = a(phi) + 1;
    a_cache[n] = val;
    return val;
}
