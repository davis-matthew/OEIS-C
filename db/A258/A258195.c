/*
 * A258195  Define a<+>b = odd part(odd part(a) + odd part(b)), where odd
 *          part(n) = A000265(n); a(n) is not already appeared prime of the
 *          form <+>_{1<=i<=k}i with the smallest k, or a(n)=0 if there is no
 *          such k (see comment).
 * https://oeis.org/A258195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, odd_part, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A258195: define f(1)=1 and for n≥2
   f(n)=odd_part( odd_part(f(n−1)) + n ).
   a(n) is the n‑th distinct odd prime that appears among the f(k)
   (if a prime never appears, a(n)=0).  The sequence is a permutation
   of the odd primes. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/odd_part.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *f_cache = NULL;      /* array of f(k) values            */
static size_t f_cap   = 0;
static size_t f_len   = 0;         /* number of values stored in f    */

static seq_t *prime_cache = NULL;  /* distinct odd primes found       */
static size_t prime_cap   = 0;
static size_t prime_len   = 0;      /* number of primes recorded       */

/* initialise caches on first use */
static void init_caches(void)
{
    if (f_cache == NULL) {
        f_cap = 16;
        f_cache = seq_alloc(f_cap);
        if (!f_cache) exit(1);
        f_cache[0] = 1;            /* f(1) = 1 */
        f_len = 1;
    }
    if (prime_cache == NULL) {
        prime_cap = 16;
        prime_cache = seq_alloc(prime_cap);
        if (!prime_cache) exit(1);
        prime_len = 0;
    }
}

/* ensure capacity for f_cache */
static void ensure_f_capacity(size_t needed)
{
    if (needed <= f_cap) return;
    size_t cap = f_cap ? f_cap << 1 : 16;
    while (cap < needed) cap <<= 1;
    seq_t *tmp = seq_realloc(f_cache, f_cap, cap);
    if (!tmp) exit(1);
    f_cache = tmp;
    f_cap   = cap;
}

/* ensure capacity for prime_cache */
static void ensure_prime_capacity(size_t needed)
{
    if (needed <= prime_cap) return;
    size_t cap = prime_cap ? prime_cap << 1 : 16;
    while (cap < needed) cap <<= 1;
    seq_t *tmp = seq_realloc(prime_cache, prime_cap, cap);
    if (!tmp) exit(1);
    prime_cache = tmp;
    prime_cap   = cap;
}

/* linear search for a prime that has already appeared */
static bool prime_seen(seq_t p)
{
    for (size_t i = 0; i < prime_len; ++i)
        if (prime_cache[i] == p) return true;
    return false;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* offset 1 – safety */

    init_caches();

    /* generate f(k) until we have collected n distinct odd primes */
    while (prime_len < (size_t)n) {
        idx_t k = (idx_t)(f_len + 1);          /* next argument to f   */

        seq_t f_prev = f_cache[f_len - 1];
        seq_t odd_f  = odd_part(f_prev);
        seq_t odd_k  = odd_part((seq_t)k);
        seq_t sum    = odd_f + odd_k;
        seq_t f_next = odd_part(sum);

        /* store f(k) */
        ensure_f_capacity(f_len + 1);
        f_cache[f_len++] = f_next;

        /* record new prime if not seen before */
        if (is_prime_big(f_next) && !prime_seen(f_next)) {
            ensure_prime_capacity(prime_len + 1);
            prime_cache[prime_len++] = f_next;
        }
    }

    return prime_cache[n - 1];
}
