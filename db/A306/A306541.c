/*
 * A306541  The least prime q such that Kronecker(D/q) >= 0 where D runs
 *          through all positive fundamental discriminants (A003658).
 * https://oeis.org/A306541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   build_primes_upto, is_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A306541: the least prime q such that Kronecker(D/q) ≥ 0,
   where D is the n‑th positive fundamental discriminant (A003658). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/jacobi_symbol.h"
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

/* ------------------------------------------------------------------ */
/*  Cache of fundamental discriminants (positive only)                 */
/* ------------------------------------------------------------------ */

static uint64_t *funds    = NULL;/* array of D values (positive)   */
static size_t   funds_len = 0;   /* number of D values stored      */
static size_t   funds_cap = 0;   /* capacity of funds array         */

/* ------------------------------------------------------------------ */
/*  List of small primes used for factorisation                       */
/* ------------------------------------------------------------------ */

static uint64_t *prime_list = NULL;/* array of primes               */
static size_t   prime_cnt   = 0;    /* number of primes in prime_list*/
static bool     primes_ready = false;

/* ------------------------------------------------------------------ */
/*  Utility: grow an array if necessary                               */
/* ------------------------------------------------------------------ */

static void grow_seq_array(uint64_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap == 0) ? 128 : (*cap * 2);
    while (new_cap < need) new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    *arr  = tmp;
    *cap  = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Push a new discriminant into the cache                            */
/* ------------------------------------------------------------------ */

static void push_fund(uint64_t d)
{
    grow_seq_array(&funds, &funds_cap, funds_len + 1);
    funds[funds_len++] = d;
}

/* ------------------------------------------------------------------ */
/*  Generate small primes for factorisation (up to 1e6)                */
/* ------------------------------------------------------------------ */

static void init_primes(void)
{
    if (primes_ready) return;
    const uint64_t LIMIT = 1000000ULL;
    build_primes_upto(LIMIT, &prime_list, &prime_cnt);
    primes_ready = true;
}

/* ------------------------------------------------------------------ */
/*  Test whether n is square‑free using the small‑prime list          */
/* ------------------------------------------------------------------ */

static bool is_squarefree(uint64_t n)
{
    if (n <= 1) return true;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) {
            n /= p;
            if (n % p == 0) return false;      /* squared factor found */
            while (n % p == 0) n /= p;          /* strip higher powers   */
        }
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Check whether d > 0 is a positive fundamental discriminant         */
/* ------------------------------------------------------------------ */
/*  Positive fundamental discriminants are:
 *   - d = 1
 *   - d ≡ 1 (mod 4) and d square‑free
 *   - d ≡ 0 (mod 4) with d/4 square‑free and d/4 ≡ 2 or 3 (mod 4)
 */
static bool is_fundamental_pos(uint64_t d)
{
    if (d == 1) return true;
    if (d % 4 == 1) {
        return is_squarefree(d);
    }
    if (d % 4 == 0) {
        uint64_t e = d / 4;
        uint64_t mod4 = e & 3ULL;          /* e % 4 */
        if (mod4 == 2 || mod4 == 3) {
            return is_squarefree(e);
        }
    }
    return false;
}

/* ------------------------------------------------------------------ */
/*  Generate fundamental discriminants up to index n (1‑based)        */
/* ------------------------------------------------------------------ */

static void ensure_funds_up_to(idx_t n)
{
    if ((size_t)n <= funds_len) return;
    init_primes();
    uint64_t cand = (funds_len == 0) ? 1 : funds[funds_len - 1] + 1;
    while (funds_len < (size_t)n) {
        if (is_fundamental_pos(cand)) push_fund(cand);
        ++cand;
    }
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined below offset */

    /* Ensure we have the nth fundamental discriminant */
    ensure_funds_up_to((idx_t) n);
    uint64_t D = funds[n - 1];

    /* Prime 2 */
    if (D % 2 == 0) return (seq_t)2;
    uint64_t mod8 = D & 7ULL;             /* D % 8 */
    if (mod8 == 1 || mod8 == 7) return (seq_t)2;

    /* Search odd primes */
    for (uint64_t q = 3; ; q += 2) {
        if (!is_prime(q)) continue;        /* skip composites */
        if (D % q == 0) return (seq_t)q;   /* ramified prime */
        int sym = jacobi_symbol(D, q);      /* Legendre symbol (D|q) */
        if (sym >= 0) return (seq_t)q;     /* splits or ramified */
    }
}
