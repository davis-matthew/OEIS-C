/*
 * A309036  a(n) = gcd(A007504(n), A014285(n)).
 * https://oeis.org/A309036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A007504(n), A014285(n)) A007504(n) = sum_{k=1..n}
 *            prime(k) A014285(n) = sum_{k=1..n} k * prime(k)
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 2188, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* ------------------------------------------------------------------ */
/* Helper for dynamic allocation compatible with C and C++         */
/* In C++ we use new/delete, in C we use malloc/free.               */
#ifndef __cplusplus
#define NEW_SEQ_ARRAY(T, n) ((T*)malloc(sizeof(T)*(n)))
#define DELETE_SEQ_ARRAY(p) free(p)
#else
#define NEW_SEQ_ARRAY(T, n) new T[(n)]
#define DELETE_SEQ_ARRAY(p) delete [] (p)
#endif

/* ------------------------------------------------------------------ */
/* Prime generation --------------------------------------------------- */
static uint64_t *primes          = NULL;   /* primes[0] == 2          */
static size_t   primes_cnt       = 0;      /* no. primes stored       */

/* ------------------------------------------------------------------ */
/* Prefix sums ------------------------------------------------------- */
static seq_t *sum_primes        = NULL;   /* sum_primes[0] = 0       */
static seq_t *sum_kp            = NULL;   /* sum_kp[0] = 0           */
static size_t sum_primes_cnt    = 0;       /* highest n computed      */

/* ------------------------------------------------------------------ */
static void ensure_primes(size_t target)
{
    if (primes_cnt >= target) return;

    /* Expand the primes array to at least target elements */
    size_t cap = (primes_cnt == 0 ? 16 : primes_cnt * 2);
    while (cap < target) cap <<= 1;
    uint64_t *newp = (uint64_t*)realloc(primes, cap * sizeof(uint64_t));
    if (!newp) abort();
    primes = newp;

    /* Start candidate from next number after last known prime or 2 */
    uint64_t candidate = (primes_cnt == 0 ? 2ULL : primes[primes_cnt-1] + 1ULL);
    while (primes_cnt < target)
    {
        if (candidate == 2ULL || (candidate & 1ULL))  /* odd numbers only */
        {
            bool is_prime = true;
            for (size_t i = 0; i < primes_cnt; ++i)
            {
                uint64_t p = primes[i];
                if (p * p > candidate) break;
                if (candidate % p == 0ULL) { is_prime = false; break; }
            }
            if (is_prime) primes[primes_cnt++] = candidate;
        }
        ++candidate;
    }
}

/* ------------------------------------------------------------------ */
static void ensure_sums(size_t target)
{
    if (sum_primes_cnt >= target) return;

    /* New capacity must be at least target+1 (include index 0) */
    size_t new_cap = target + 1;
    seq_t *new_sum_primes = NEW_SEQ_ARRAY(seq_t, new_cap);
    seq_t *new_sum_kp     = NEW_SEQ_ARRAY(seq_t, new_cap);
    if (!new_sum_primes || !new_sum_kp) abort();

    /* Index 0 initialises to 0 */
    new_sum_primes[0] = 0;
    new_sum_kp[0]     = 0;

    /* Copy over old prefix sums if any */
    for (size_t i = 1; i <= sum_primes_cnt; ++i)
    {
        new_sum_primes[i] = sum_primes[i];
        new_sum_kp[i]     = sum_kp[i];
    }

    /* Compute new entries */
    for (size_t i = sum_primes_cnt + 1; i <= target; ++i)
    {
        uint64_t p = primes[i-1];
        new_sum_primes[i] = new_sum_primes[i-1] + (seq_t)p;
        new_sum_kp[i]     = new_sum_kp[i-1] + (seq_t)i * (seq_t)p;
    }

    /* Replace old arrays with the new ones */
    DELETE_SEQ_ARRAY(sum_primes);
    DELETE_SEQ_ARRAY(sum_kp);
    sum_primes   = new_sum_primes;
    sum_kp       = new_sum_kp;
    sum_primes_cnt = target;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* The OEIS offset is 1: a(1) is first term */
    if (n == 0) return 0;

    size_t idx = (size_t)n;
    ensure_primes(idx);
    ensure_sums(idx);

    return gcd(sum_primes[idx], sum_kp[idx]);
}
