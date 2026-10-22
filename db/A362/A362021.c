/*
 * A362021  a(n) = Sum_{k=1..n} (-1)^(n-k) * k * mu(k), where mu(k) is the
 *          Moebius function.
 * https://oeis.org/A362021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, look
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(n-k) * k * mu(k)
 * Range:     i8 n <= 25, i16 n <= 2473, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>


/* --------------------------------------------------------------------- */
/*  Linear sieve for the Möbius function mu(k) for 1 <= k <= limit       */
/* --------------------------------------------------------------------- */
static int   *mu      = NULL;   /* values -1, 0, 1 for k */
static unsigned char *vis = NULL; /* visited flag used in the sieve */
static uint32_t *primes  = NULL; /* list of primes found so far */
static size_t mu_cap    = 0;      /* current allocated size of mu/vis  */
static size_t primes_cap    = 0;
static size_t primes_cnt    = 0;

/* Reallocate an array to accommodate at least 'needed' elements.     */
static void ensure_cap(size_t *cap_ptr, void **ptr, size_t elem_size,
                       size_t needed) {
    size_t new_cap = *cap_ptr;
    if (new_cap < needed)
        new_cap = needed > 2 ? needed * 2 : 2;
    *ptr = realloc(*ptr, new_cap * elem_size);
    if (!*ptr) abort();                     /* out of memory */
    *cap_ptr = new_cap;
}

/* Pre‑compute mu(k) for all 1 <= k <= n (inclusive).                */
static void ensure_mu_up_to(idx_t n) {
    if (n < 1) return;
    size_t need = (size_t)n + 1;      /* index n */

    /* Resize arrays if necessary. */
    if (need > mu_cap) {
        ensure_cap(&mu_cap,   (void **)&mu,    sizeof(int),   need);
        ensure_cap(&mu_cap,   (void **)&vis,   sizeof(unsigned char), need);
        ensure_cap(&primes_cap, (void **)&primes, sizeof(uint32_t),
                   need / 2 + 1);

        /* Clear the visited flags for the newly allocated region.   */
        for (size_t i = mu_cap; i < need; ++i)
            vis[i] = 0;
        mu_cap = need;
    }

    /* Clear all visited flags so that the sieve starts cleanly.  */
    for (size_t i = 2; i < need; ++i)
        vis[i] = 0;

    mu[1] = 1;            /* mu(1) = 1 */
    primes_cnt = 0;

    /* Classic linear (Euler) sieve to compute mu[]. */
    for (size_t i = 2; i < need; ++i) {
        if (!vis[i]) {
            primes[primes_cnt++] = (uint32_t)i;
            mu[(size_t)i] = -1;   /* prime */
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint64_t p   = primes[j];
            uint64_t ip  = i * p;
            if (ip >= need) break;
            vis[(size_t)ip] = 1;
            if (i % p == 0) {
                mu[(size_t)ip] = 0; /* not square‑free */
                break;
            } else {
                mu[(size_t)ip] = -mu[(size_t)i];
            }
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Sequence a(n) according to definition                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;

    ensure_mu_up_to(n);

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        int m = mu[(size_t)k];
        if (m == 0) continue;      /* contribute nothing */
        seq_t term = (seq_t)k * (seq_t)m;
        /* (-1)^(n-k) -> add if even, subtract if odd            */
        if (((n - k) & 1) == 0)
            sum += term;
        else
            sum -= term;
    }
    return sum;
}
