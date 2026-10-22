/*
 * A382091  a(1) = 1, a(2) = 2; for n > 2, a(n) is the smallest unused
 *          positive number such that a(n) shares a factor with a(n-1) while
 *          the total number of prime terms of the form 4*k + 1 is never less
 *          than those of the form 4*k + 3.
 * https://oeis.org/A382091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 162, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A382091
   a(1)=1, a(2)=2. For n>2 a(n) is the smallest unused positive integer
   that shares a factor with a(n-1) and for which the total number of
   primes congruent to 1 (mod 4) never falls below that of primes
   congruent to 3 (mod 4). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"

/* static state --------------------------------------------------------------*/
static seq_t *seq = NULL;            /* array of sequence values      */
static size_t seq_cap = 0;           /* capacity of seq[]             */
static size_t seq_len = 0;           /* number of values stored       */

static bool *used = NULL;            /* bitmap of numbers already used*/
static size_t used_cap = 0;          /* capacity of used[]            */

static int count1 = 0;               /* primes ≡1 (mod 4) so far      */
static int count3 = 0;               /* primes ≡3 (mod 4) so far      */

/* ensure at least `need` elements in seq[] ---------------------------------*/
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ensure at least `need` elements in used[] ---------------------------------*/
static void ensure_used_cap(size_t need)
{
    if (need <= used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    used = (bool *)realloc(used, new_cap * sizeof(bool));
    memset(used + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used_cap = new_cap;
}

/* distinct prime factors of a 64‑bit number ---------------------------------*/
static size_t distinct_factors(uint64_t n, uint64_t *fac, size_t maxfac)
{
    size_t cnt = 0;
    if (n % 2 == 0) {
        if (cnt < maxfac) fac[cnt++] = 2;
        while (n % 2 == 0) n /= 2;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            if (cnt < maxfac) fac[cnt++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1 && cnt < maxfac) fac[cnt++] = n;
    return cnt;
}

/* --------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* safe guard – not part of the sequence */

    /* initialise on first call -------------------------------------------*/
    if (seq_len == 0) {
        ensure_seq_cap(4);
        ensure_used_cap(4);
        seq[0] = (seq_t)1;  used[(size_t)1] = true;
        seq[1] = (seq_t)2;  used[(size_t)2] = true;
        seq_len = 2;
    }

    /* generate terms up to n ---------------------------------------------*/
    while (seq_len < (size_t)n) {
        uint64_t prev = (uint64_t)seq[seq_len - 1];

        /* collect distinct prime factors of prev */
        uint64_t fac[64];
        size_t faccnt = distinct_factors(prev, fac, 64);

        uint64_t best = (uint64_t)-1;  /* sentinel for minimal candidate */

        /* try each prime factor, seeking the smallest admissible multiple */
        for (size_t i = 0; i < faccnt; ++i) {
            uint64_t p = fac[i];
            uint64_t cand = p;           /* smallest multiple of p > 0 */
            for (;;) {
                ensure_used_cap(cand + 1);
                if (used[(size_t)cand]) {            /* already used */
                    cand += p;
                    continue;
                }
                /* if cand is a prime, enforce the 4k+1/4k+3 rule */
                if (is_prime(cand)) {
                    uint64_t mod4 = cand & 3;
                    if (mod4 == 3 && count1 <= count3) {
                        cand += p;
                        continue;
                    }
                }
                break;                                  /* candidate accepted */
            }
            if (cand < best) best = cand;
        }

        /* store the selected term */
        ensure_seq_cap(seq_len + 1);
        ensure_used_cap(best + 1);
        seq[seq_len++] = (seq_t)best;
        used[(size_t)best] = true;

        /* update prime counters if the new term is prime */
        if (is_prime(best)) {
            if ((best & 3) == 1) ++count1;
            else if ((best & 3) == 3) ++count3;
        }
    }

    return seq[(size_t)n - 1];
}
