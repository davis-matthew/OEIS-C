/*
 * A355667  Least number phi(k) such that n * phi(k) < k, where phi is Euler's
 *          totient function.
 * https://oeis.org/A355667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p≤p_k} (p-1) where p_k is the smallest prime such that
 *            * ∏_{p≤p_k} p/(p-1) > n * * This follows directly from the
 *            definition: * a(n) = φ( m ) where m is the smallest integer
 *            satisfying n·φ(m) < m. * For m squarefree the ratio m/φ(m)
 *            depends only on its distinct prime factors: * m/φ(m) =...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   next_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/next_prime.h"   /* next_prime(uint64_t) -> uint64_t   */
#include "../utils/seq_mul.h"      /* seq_mul(seq_t a, seq_t b) -> seq_t */

/* static storage for pre‑computed values */
static uint32_t *primes      = NULL;   /* primes[0] = 2, primes[1] = 3, ...          */
static seq_t   *phi_prefix  = NULL;   /* phi_prefix[i] = ∏_(j=0..i) (primes[j]-1)   */
static long double *ratio    = NULL;   /* ratio[i] = ∏_(j=0..i) primes[j]/(primes[j]-1) */

static size_t prime_cnt   = 0;         /* number of primes stored            */
static size_t prime_cap   = 0;         /* current allocation capacity        */
static seq_t   last_phi   = 0;         /* φ(product of all stored primes)   */
static long double last_ratio = 1.0L;   /* product of p/(p-1) of stored primes */

static idx_t  max_n_computed = 0;     /* largest n for which a(n) is guaranteed known */

/* enlarge storage for at least 'need' entries */
static void enlarge_storage(size_t need)
{
    if (prime_cap >= need) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;

    primes   = (uint32_t*)realloc(primes,   new_cap * sizeof(uint32_t));
    phi_prefix = (seq_t*)realloc(phi_prefix, new_cap * sizeof(seq_t));
    ratio    = (long double*)realloc(ratio,    new_cap * sizeof(long double));
    prime_cap = new_cap;
}

/* initialise the table with the first prime (2) */
static void init_table(void)
{
    enlarge_storage(1);

    uint64_t p = 2ULL;
    primes[0] = (uint32_t)p;
    /* φ(2) = 1  */
    phi_prefix[0] = (seq_t)(p - 1);
    last_phi = phi_prefix[0];
    /* ratio(2) = 2/(2-1) = 2 */
    ratio[0] = (long double)p / (long double)(p - 1);
    last_ratio = ratio[0];

    prime_cnt = 1;
}

/* ensure all primes needed to cover n are present */
static void ensure_primes_upto(idx_t n)
{
    if (prime_cnt == 0) init_table();

    while (last_ratio <= (long double)n)
    {
        /* get next prime after the last stored */
        uint64_t next_p = next_prime((uint64_t)primes[prime_cnt - 1]);

        enlarge_storage(prime_cnt + 1);

        primes[prime_cnt] = (uint32_t)next_p;

        /* new φ prefix = previous * (p-1) */
        seq_t new_phi = seq_mul(last_phi, (seq_t)(next_p - 1));
        phi_prefix[prime_cnt] = new_phi;
        last_phi = new_phi;

        /* update ratio */
        long double new_ratio = last_ratio * (long double)next_p / (long double)(next_p - 1);
        ratio[prime_cnt] = new_ratio;
        last_ratio = new_ratio;

        ++prime_cnt;
    }
}

/* binary search: first index i with ratio[i] > n */
static size_t find_index(idx_t n)
{
    size_t lo = 0, hi = prime_cnt - 1;
    while (lo < hi)
    {
        size_t mid = (lo + hi) >> 1;
        if (ratio[mid] > (long double)n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* --------------------------------------------------------------------- */
/*  public function required by the task                                 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* off‑by‑one case; sequence starts at n=1 */

    /* compute up to n if not already known */
    if (n > max_n_computed)
    {
        ensure_primes_upto(n);
        max_n_computed = n;
    }

    /* locate the prefix that gives the first ratio greater than n */
    size_t idx = find_index(n);
    return phi_prefix[idx];
}
