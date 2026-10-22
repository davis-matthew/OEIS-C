/*
 * A392117  Lower Wythoff partition sequence of the primes; see Comments.
 * https://oeis.org/A392117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A392117 – Lower Wythoff partition sequence of the primes
 *
 * s(n)  = n-th prime
 * u(1)  = s(1)
 * v(1)  = least prime ≥ u(1)+s(1)
 * u(n)  = least prime not equal to any previous u(k) or v(k)
 * v(n)  = least prime ≥ u(n)+s(n)
 *
 * a(n) = u(n)
 *
 * This implementation generates primes on demand, keeps a bit‑array of
 * used primes, and caches computed u values.  All operations use only
 * integer arithmetic and the build‑provided int_sqrt function.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"  /* int_sqrt(idx_t) */

static idx_t *primes      = NULL;  /* list of primes */
static size_t primes_sz   = 0;     /* number of primes stored */
static size_t primes_cap  = 0;     /* prime array capacity */

static bool *used   = NULL;        /* used[p] == true if prime p already in u or v */
static size_t used_cap = 0;        /* size of used array (must be >= max prime + 1) */

static seq_t *u_seq = NULL;       /* cached u(n) values (a(n)) */
static size_t u_len = 0;          /* number of u values cached */
static size_t u_cap = 0;          /* u_seq capacity */

static idx_t next_candidate = 2;   /* candidate for next prime generation */

static idx_t prime_index_for_u = 0; /* next index in primes[] to search for u */
static idx_t prime_index_for_v = 0; /* next index in primes[] to search for v */

/* --------------------------------------------------------------------- */
/*  Utility functions for growing dynamic arrays                         */
/* --------------------------------------------------------------------- */
static void grow_primes_cap(size_t needed) {
    if (primes_cap >= needed) return;
    size_t new_cap = primes_cap ? primes_cap << 1U : 16U;
    while (new_cap < needed) new_cap <<= 1U;
    idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
    if (!tmp) exit(1);
    primes = tmp;
    primes_cap = new_cap;
}

static void grow_used_cap(idx_t needed) {
    if (used_cap > needed) return;
    size_t new_cap = used_cap ? used_cap << 1U : 16U;
    while (new_cap <= needed) new_cap <<= 1U;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp) exit(1);
    for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = false;
    used = tmp;
    used_cap = new_cap;
}

static void grow_u_seq(size_t needed) {
    if (u_cap >= needed) return;
    size_t new_cap = u_cap ? u_cap << 1U : 16U;
    while (new_cap < needed) new_cap <<= 1U;
    seq_t *tmp = (seq_t *)realloc(u_seq, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    u_seq = tmp;
    u_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/*  Prime generation (trial division using previously found primes)     */
/* --------------------------------------------------------------------- */
static void generate_next_prime(void) {
    idx_t cand = next_candidate;
    if (cand <= 2) cand = 2;

    for (;;) {
        bool prime = true;
        idx_t limit = int_sqrt(cand);
        for (size_t i = 0; i < primes_sz && primes[i] <= limit; ++i) {
            if (cand % primes[i] == 0) {
                prime = false;
                break;
            }
        }
        if (prime) break;
        ++cand;
    }

    grow_primes_cap(primes_sz + 1);
    primes[primes_sz++] = cand;

    grow_used_cap(cand + 1);   /* ensure array covers this index */
    used[cand] = false;        /* prime is currently unused */

    next_candidate = cand + 1;
}

/* --------------------------------------------------------------------- */
/*  Compute the sequence up to nmax (inclusive)                         */
/* --------------------------------------------------------------------- */
static void compute_up_to(idx_t nmax) {
    while (u_len < nmax) {
        /* -----------------------------------------------------------------
         *  Find next unused prime for u
         * ----------------------------------------------------------------- */
        while (prime_index_for_u >= primes_sz) generate_next_prime();

        while (prime_index_for_u < primes_sz && used[primes[prime_index_for_u]]) {
            ++prime_index_for_u;
        }
        if (prime_index_for_u >= primes_sz) {
            generate_next_prime();          /* grow primes and retry */
            continue;
        }
        idx_t u_val = primes[prime_index_for_u];
        used[u_val] = true;
        ++prime_index_for_u;

        /* Store the new u value */
        grow_u_seq(u_len + 1);
        u_seq[u_len++] = (seq_t)u_val;

        /* -----------------------------------------------------------------
         *  Compute s(n) = n-th prime (now u_len is n)
         * ----------------------------------------------------------------- */
        while (primes_sz < u_len) generate_next_prime();   /* ensure we have n primes */

        idx_t s_val = primes[u_len - 1];                 /* nth prime */
        idx_t target = u_val + s_val;                    /* value for v */

        /* -----------------------------------------------------------------
         *  Find least prime ≥ target
         * ----------------------------------------------------------------- */
        while (primes_sz == 0 || primes[primes_sz - 1] < target)
            generate_next_prime();                        /* extend primes as needed */

        while (prime_index_for_v < primes_sz && primes[prime_index_for_v] < target)
            ++prime_index_for_v;

        while (prime_index_for_v >= primes_sz) generate_next_prime();  /* safety */

        while (prime_index_for_v < primes_sz && primes[prime_index_for_v] < target)
            generate_next_prime();                    /* just in case */

        idx_t v_val = primes[prime_index_for_v];
        used[v_val] = true;
        ++prime_index_for_v;
    }
}

/* --------------------------------------------------------------------- */
/*  Public entry point: a(n)                                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    static bool initialized = false;
    if (!initialized) {
        primes        = NULL;
        primes_sz     = 0;
        primes_cap    = 0;
        used          = NULL;
        used_cap      = 0;
        u_seq         = NULL;
        u_len         = 0;
        u_cap         = 0;
        next_candidate = 2;
        prime_index_for_u = 0;
        prime_index_for_v = 0;
        initialized = true;
    }
    if (n == 0) return 0;
    compute_up_to(n);
    return u_seq[n - 1];
}
