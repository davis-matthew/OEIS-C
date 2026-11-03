/*
 * A243348  Difference between the n-th squarefree number and n: a(n) =
 *          A005117(n) - n.
 * https://oeis.org/A243348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005117(n) - n (nth square‑free number minus n)
 * Range:     u8 n <= 39, u16 n <= 8641, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 89 terms through n = 8641 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* --------------------------------------------------------------------- */
/* Static cache structures                                              */
/* --------------------------------------------------------------------- */
static seq_t *sf = NULL;          /* stored square‑free numbers (0‑based) */
static idx_t  sf_cnt = 0;         /* number of elements stored          */
static idx_t  sf_cap = 0;         /* allocation capacity for sf          */

static idx_t *primes = NULL;      /* prime list (up to sqrt of the current candidate) */
static idx_t  primes_cnt = 0;     /* number of primes stored            */
static idx_t  primes_cap = 0;     /* allocation capacity for primes     */
static idx_t  last_prime = 1;     /* last prime considered for sieving */

/* --------------------------------------------------------------------- */
/* Helper: ensure the prime list contains all primes ≤ `limit`.          */
/* --------------------------------------------------------------------- */
static void ensure_primes_up_to(idx_t limit)
{
    while (last_prime < limit)
    {
        idx_t cand = last_prime + 1;
        bool is_prime = true;
        for (idx_t i = 0; i < primes_cnt; ++i)
        {
            idx_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime)
        {
            if (primes_cnt == primes_cap)
            {
                primes_cap = primes_cap ? primes_cap * 2 : 8;
                primes = (idx_t*)realloc(primes, primes_cap * sizeof(idx_t));
            }
            primes[primes_cnt++] = cand;
        }
        last_prime = cand;
    }
}

/* --------------------------------------------------------------------- */
/* Helper: test whether a given seq_t number is square‑free.             */
/* --------------------------------------------------------------------- */
static bool is_squarefree(const seq_t val)
{
    seq_t sqrt_val = floor_sqrt(val);
    idx_t sqrt_lim = (idx_t)sqrt_val;    /* safe for the test range */

    ensure_primes_up_to(sqrt_lim);

    for (idx_t i = 0; i < primes_cnt; ++i)
    {
        idx_t p   = primes[i];
        seq_t p2  = (seq_t)p * (seq_t)p;
        if (p2 > val) break;                /* remaining primes cannot divide */
        if (val % p2 == 0) return false;    /* divisible by a square */
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Main function: compute A243348(a(n)).                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* defensive; OEIS offset starts at 1 */

    /* Allocate or enlarge the square‑free cache if necessary. */
    if (sf_cap == 0)
    {
        sf_cap = 64;
        sf = mem_new(seq_t, sf_cap);
    }

    while (sf_cnt < n)
    {
        /* Generate next square‑free integer. */
        seq_t cand;
        if (sf_cnt == 0)
            cand = (seq_t)1;
        else
            cand = sf[sf_cnt - 1] + (seq_t)1;

        /* Ensure we have all primes up to sqrt(cand). */
        seq_t sqrt_cand = floor_sqrt(cand);
        idx_t sqrt_lim = (idx_t)sqrt_cand;
        ensure_primes_up_to(sqrt_lim);

        /* Find the next square‑free number. */
        while (!is_squarefree(cand))
            cand = cand + (seq_t)1;

        /* Store it. */
        if (sf_cnt == sf_cap)
        {
            sf_cap = sf_cap ? sf_cap * 2 : 64;
            sf = mem_resize(seq_t, sf, sf_cap);
        }
        sf[sf_cnt++] = cand;
    }

    /* a(n) = nth square‑free number – n. */
    return sf[n - 1] - (seq_t)n;
}
