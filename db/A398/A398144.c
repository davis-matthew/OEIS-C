/*
 * A398144  a(n) is the number of pairs (i,j) with mu(i) = mu(j), 1 <= i,j <=
 *          n, where mu = A008683 (Moebius function).
 * https://oeis.org/A398144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 440, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A398144.
   Formula: a(n) = (#μ=1)^2 + (#μ=0)^2 + (#μ=-1)^2.
   The routine recomputes the Möbius values up to the requested n
   using a linear sieve (O(n)).  The counts of each value are then
   combined to obtain a(n). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- Cached data for sieve ---------------------------------------------- */
static int8_t  *mu_cache   = NULL;      /* Möbius values: -1,0,1           */
static size_t  mu_cap     = 0;

static uint32_t *spf_cache   = NULL;   /* smallest prime factor          */
static size_t  spf_cap     = 0;

static uint32_t *primes_cache = NULL;  /* list of primes up to limit     */
static size_t  primes_cap   = 0;
static size_t  primes_cnt   = 0;

/* --------------------------------------------------------------------- */
/* Recompute the Möbius values (and the SPF table) up to at least n.     */
static void recompute_mu(idx_t n)
{
    if (n < mu_cap)                /* already computed */
        return;

    /* Allocate new arrays.  Use calloc to zero initialize.            */
    size_t new_cap = (size_t)(n + 1);
    int8_t  *new_mu  = (int8_t  *)calloc(new_cap, sizeof(int8_t));
    uint32_t *new_spf = (uint32_t *)calloc(new_cap, sizeof(uint32_t));
    if (!new_mu || !new_spf)       /* allocation failure */
        exit(1);

    new_mu[1] = 1;                  /* μ(1) = 1 */
    new_spf[1] = 1;

    /* Ensure prime list buffer is large enough.                       */
    if (primes_cap < new_cap) {
        size_t new_primes_cap = new_cap;
        uint32_t *tmp =
            (uint32_t *)realloc(primes_cache, new_primes_cap * sizeof(uint32_t));
        if (!tmp)
            exit(1);
        primes_cache = tmp;
        primes_cap   = new_primes_cap;
    }

    primes_cnt = 0;

    for (idx_t i = 2; i <= n; ++i) {
        if (new_spf[i] == 0) {        /* i is prime                      */
            new_spf[i] = (uint32_t)i;
            new_mu[i]  = -1;          /* μ(p) = -1                       */
            primes_cache[primes_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            idx_t p  = primes_cache[j];
            idx_t ip = i * p;
            if (ip > n)
                break;
            new_spf[ip] = (uint32_t)p;
            if (i % p == 0) {         /* square factor, μ = 0            */
                new_mu[ip] = 0;
                break;
            } else {                   /* μ(ip) = -μ(i)                   */
                new_mu[ip] = (int8_t)(-new_mu[i]);
            }
        }
    }

    /* Replace old buffers with new ones.  Free the old arrays.          */
    free(mu_cache);
    free(spf_cache);
    mu_cache  = new_mu;
    spf_cache = new_spf;
    mu_cap    = new_cap;
    spf_cap   = new_cap;
}

/* --------------------------------------------------------------------- */
/* Main routine:  a(n) = number of pairs (i,j) with μ(i)=μ(j) for
   1 ≤ i,j ≤ n.                                                    */
seq_t a(idx_t n)
{
    if (n == 0)                /* sequence offset starts at 1 */
        return (seq_t)0;

    recompute_mu(n);

    seq_t cnt_pos  = 0;   /* μ = +1   */
    seq_t cnt_zero = 0;   /* μ =  0   */
    seq_t cnt_neg  = 0;   /* μ = -1   */

    for (idx_t i = 1; i <= n; ++i) {
        int8_t v = mu_cache[(size_t)i];
        if (v == 1)
            ++cnt_pos;
        else if (v == 0)
            ++cnt_zero;
        else                       /* v == -1 */
            ++cnt_neg;
    }

    return cnt_pos * cnt_pos + cnt_zero * cnt_zero + cnt_neg * cnt_neg;
}
