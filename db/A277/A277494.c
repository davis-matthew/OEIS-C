/*
 * A277494  a(n) = smallest m for which there is a sequence n = b_1 < b_2 <=
 *          b_3 <= ... <= b_t = m such that b_1*b_2*...*b_t is a perfect cube.
 * https://oeis.org/A277494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m ≥ n such that the GF(3)-vector of n (prime
 *            exponents modulo 3) belongs to the span of the vectors of
 *            n+1,…,m. Computed by iteratively building a basis over GF(3)
 * Range:     u8 n <= 130, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* --- Constants and global tables ---------------------------- */

#define LIMIT        50000   /* must cover all n used in tests */
#define MAX_PRIMES   6000    /* upper bound on number of primes ≤ LIMIT */
#define MAX_FACTOR   12      /* maximum distinct prime factors per n */

/* Smallest prime‑factor table and primes list */
static uint32_t spf[LIMIT + 1];          /* spf[i] = smallest prime factor of i (or i itself) */
static uint32_t primes[MAX_PRIMES];      /* list of primes ≤ LIMIT */
static uint32_t prime_index[LIMIT + 1];  /* prime_index[p] = index of prime p in primes[] */

/* ------------------------------------------------------------ */
/* --- Vector over GF(3) -------------------------------------- */

typedef struct {
    uint8_t  len;                      /* number of non‑zero entries   */
    uint32_t prime[MAX_FACTOR];        /* prime values (not indices)   */
    uint8_t  coeff[MAX_FACTOR];        /* coefficients 1 or 2 modulo 3*/
} Vec;

/* Vectors for all numbers up to LIMIT */
static Vec vecs[LIMIT + 1];

/* ----------------------------------------------------------------- */
/* --- Cache for already computed a(n) ------------------------------ */

static seq_t *cache        = NULL;   /* values of a(n)           */
static bool  *cache_done   = NULL;   /* cache flags              */
static size_t cache_size   = 0;      /* current cache allocation */

/* ----------------------------------------------------------------- */
/* --- Sieve of Eratosthenes ------------------------------------- */

static void init_sieve(void)
{
    for (uint32_t i = 0; i <= LIMIT; ++i) {
        spf[i]        = 0;
        prime_index[i] = 0;
    }
    spf[0] = spf[1] = 1;
    uint32_t cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = i;
            if (cnt < MAX_PRIMES) primes[cnt++] = i;
            prime_index[i] = cnt - 1;
            if ((uint64_t)i * i <= LIMIT)
                for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += i)
                    if (spf[(uint32_t)j] == 0) spf[(uint32_t)j] = i;
        }
    }
    /* mark remaining numbers as prime (they are themselves) */
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (spf[i] == 0) spf[i] = i;
}

/* ----------------------------------------------------------------- */
/* --- Factorization modulo 3 ------------------------------------- */

/* Build the GF(3) vector for integer n and store in *v */
static void factor_mod3(uint32_t n, Vec *v)
{
    v->len = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        uint32_t exp = 0;
        while (n % p == 0) {
            n /= p;
            ++exp;
        }
        uint8_t e = exp % 3;
        if (e) {
            if (v->len < MAX_FACTOR) {
                v->prime[v->len]   = p;
                v->coeff[v->len]   = e;   /* either 1 or 2 */
                ++v->len;
            }
        }
    }
}

/* Find index of prime p in vector v, or -1 if absent */
static int idx_in_vec(const Vec *v, uint32_t p)
{
    for (uint8_t i = 0; i < v->len; ++i)
        if (v->prime[i] == p) return (int)i;
    return -1;
}

/* v -= c * b  (mod 3) ;   modifies v in place */
static void subtract_vec(Vec *v, const Vec *b, uint8_t c)
{
    if (c == 0) return;
    for (uint8_t i = 0; i < b->len; ++i) {
        uint32_t p = b->prime[i];
        uint8_t   bp = b->coeff[i];
        uint8_t   sub = (uint8_t)((c * bp) % 3);
        if (sub == 0) continue;
        int pos = idx_in_vec(v, p);
        if (pos == -1) {                 /* new entry, -sub mod 3 = 3-sub */
            uint8_t nc = (3 - sub) % 3;
            if (v->len < MAX_FACTOR) {
                v->prime[v->len] = p;
                v->coeff[v->len] = nc;
                ++v->len;
            }
        } else {
            uint8_t nc = (uint8_t)((v->coeff[pos] + 3 - sub) % 3);
            if (nc == 0) {
                /* delete element */
                for (int k = pos; k + 1 < (int)v->len; ++k) {
                    v->prime[k]   = v->prime[k + 1];
                    v->coeff[k]   = v->coeff[k + 1];
                }
                --v->len;
            } else {
                v->coeff[pos] = nc;
            }
        }
    }
}

/* Reduce v against the current basis (array of Vec), in place */
static void reduce_vec(Vec *v, const Vec *basis, int basisSize)
{
    for (int i = 0; i < basisSize; ++i) {
        if (v->len == 0) break;
        int pos = idx_in_vec(v, basis[i].prime[0]);  /* leading prime of basis[i] */
        if (pos == -1) continue;
        uint8_t c = v->coeff[pos];
        subtract_vec(v, &basis[i], c);
    }
}

/* Insert a non‑zero vector into basis after normalizing its leading coeff. */
static void insert_basis(const Vec *src, Vec *basis, int *basisSize)
{
    Vec v = *src;                    /* make copy */
    /* Normalize leading coefficient to 1.  In GF(3), inverse of 2 is 2. */
    uint8_t inv = (v.coeff[0] == 1) ? 1 : 2;
    if (inv != 1) {
        for (uint8_t i = 0; i < v.len; ++i)
            v.coeff[i] = (uint8_t)((v.coeff[i] * inv) % 3);
    }
    basis[(*basisSize)++] = v;
}

/* ----------------------------------------------------------------- */
/* --- Main function: a(n) ---------------------------------------- */

seq_t a(idx_t n)
{
    /* one‑time initialization */
    static bool once = false;
    if (!once) {
        once = true;
        init_sieve();
        /* precompute vectors for all numbers */
        for (uint32_t i = 0; i <= LIMIT; ++i) factor_mod3(i, &vecs[i]);

        /* initialise cache */
        cache_size = 4;
        cache      = (seq_t*)malloc(sizeof(seq_t) * cache_size);
        cache_done = (bool*)malloc(sizeof(bool)   * cache_size);
        for (size_t i = 0; i < cache_size; ++i) cache_done[i] = false;
    }

    /* grow cache if needed */
    if (n >= cache_size) {
        size_t new_sz = (n + 1) * 2;
        cache      = (seq_t*)realloc(cache,      sizeof(seq_t)  * new_sz);
        cache_done = (bool*)realloc(cache_done,  sizeof(bool) * new_sz);
        for (size_t i = cache_size; i < new_sz; ++i) cache_done[i] = false;
        cache_size = new_sz;
    }

    if (cache_done[n]) return cache[n];

    /* Handle trivial first two values */
    if (n < 2) {
        cache[n] = (seq_t)n;
        cache_done[n] = true;
        return cache[n];
    }

    /* If n is very large (beyond precomputed LIMIT) fall back to n itself */
    if ((uint64_t)n > LIMIT) {
        cache[n] = (seq_t)n;
        cache_done[n] = true;
        return cache[n];
    }

    const Vec *base_vec = &vecs[(uint32_t)n];

    /* If n itself is a perfect cube it needs no extension */
    if (base_vec->len == 0) {
        cache[n] = (seq_t)n;
        cache_done[n] = true;
        return cache[n];
    }

    /* Basis for numbers greater than current n */
    static Vec basis[MAX_PRIMES];
    int basisSize = 0;

    seq_t answer = (seq_t)n;          /* default: n (should never happen) */

    for (uint32_t m = (uint32_t)n + 1; m <= LIMIT; ++m) {
        /* Add vector of m to basis if it is non‑zero */
        const Vec *v_m = &vecs[m];
        if (v_m->len > 0) {
            Vec copy = *v_m;
            reduce_vec(&copy, basis, basisSize);
            if (copy.len > 0) insert_basis(&copy, basis, &basisSize);
        }

        /* Check whether base vector is in span of basis */
        Vec tmp = *base_vec;
        reduce_vec(&tmp, basis, basisSize);
        if (tmp.len == 0) {
            answer = (seq_t)m;
            break;
        }
    }

    /* Store result */
    cache[n]      = answer;
    cache_done[n] = true;
    return answer;
}
