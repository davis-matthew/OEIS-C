/*
 * A390099  Record denominators of (p2+1)/(p1+1), where p1 < p2 are
 *          consecutive primes.
 * https://oeis.org/A390099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 3354, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A390099:  record denominators of (p_{k+1}+1)/(p_k+1),
   where p_k , p_{k+1} are consecutive primes.  The nth term is the
   nth denominator that exceeds all previous ones.  The implementation
   generates primes incrementally and keeps only the record denominators,
   so any call to a(n) runs in O(n) time.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* prime list ------------------------------------------------------------- */
static uint32_t *primes = NULL;          /* primes[0] … primes[primes_cnt-1] */
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

/* record denominators ----------------------------------------------------- */
static seq_t *recdenoms = NULL;           /* recdenoms[0] … recdenoms[rec_cnt-1] */
static size_t rec_cnt = 0;
static size_t rec_cap = 0;
static seq_t rec_max = 0;

/* ------------------------------------------------------------------------ */
static void ensure_prime_cap(void)
{
    if (primes_cnt == primes_cap) {
        primes_cap = (primes_cap == 0) ? 16 : primes_cap << 1;
        primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
    }
}

/* ------------------------------------------------------------------------ */
static void ensure_record_cap(void)
{
    if (rec_cnt == rec_cap) {
        rec_cap = (rec_cap == 0) ? 16 : rec_cap << 1;
        recdenoms = (seq_t *)realloc(recdenoms, rec_cap * sizeof(seq_t));
    }
}

/* ------------------------------------------------------------------------ */
static bool is_prime_from_list(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = (uint64_t)primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------------ */
/* generate primes and record denominators until rec_cnt >= needed */
static void fill_until(idx_t needed)
{
    if (needed <= rec_cnt) return;

    /* initialise prime list if empty */
    if (primes_cnt == 0) {
        primes_cap = 16;
        primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
        primes[0] = 2;
        primes_cnt = 1;
    }

    while (rec_cnt < (size_t)needed) {
        /* find next prime after the last one */
        uint64_t cand = (uint64_t)primes[primes_cnt - 1] + 1;
        for (;;) {
            if (is_prime_from_list(cand)) break;
            ++cand;
        }

        /* store new prime */
        ensure_prime_cap();
        primes[primes_cnt++] = (uint32_t)cand;

        /* compute the denominator d = (p_{k} + 1) / gcd(p_{k+1}+1, p_{k}+1) */
        seq_t p1_plus = (seq_t)((uint64_t)primes[primes_cnt - 2] + 1U);
        seq_t p2_plus = (seq_t)(cand + 1U);
        seq_t g = gcd(p1_plus, p2_plus);
        seq_t denom = p1_plus / g;

        /* record if this is a new maximum denominator */
        if (denom > rec_max) {
            ensure_record_cap();
            recdenoms[rec_cnt++] = denom;
            rec_max = denom;
        }
    }
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n<1; return 0 */
    fill_until(n);
    return recdenoms[(size_t)(n - 1)];
}
