/*
 * A331137  a(n) = Sum_{primes p <= n} b(p-1), where b = A108548.
 * https://oeis.org/A331137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{primes p <= n} b(p-1), where b(k) is the number of
 *            unordered integer solutions of k = a^2 + b^2 with a >= b >= 0
 *            (counts each representation once)
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   build_primes_upto, sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---- standard includes ---- */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
/* ---- helper includes from the utils library ---- */
#include "../utils/build_primes_upto.h"
#include "../utils/sqrt_floor.h"

/* ---- static storage for prime generation ---- */
static uint64_t *primes     = NULL;   /* array of primes */
static size_t   primes_cnt  = 0;      /* number of primes */
static idx_t    primes_lim  = 0;      /* largest prime that has been prime‑sieved */

/* ---- ensure we have all primes up to 'limit' ---- */
static void
ensure_primes_upto(idx_t limit)
{
    if (limit <= primes_lim) return;               /* already satisfied */

    /* free previously allocated array */
    if (primes) free(primes);

    /* build primes up to 'limit' */
    uint64_t *tmp = NULL;
    size_t   cnt  = 0;
    /* build_primes_upto is provided by the utils package */
    build_primes_upto(limit, &tmp, &cnt);

    primes     = tmp;
    primes_cnt = cnt;
    primes_lim = limit;
}

/* ---- compute b(k) as defined above ---- */
static seq_t
b_of(uint64_t k)
{
    if (k == 0) return 1;  /* 0 = 0^2 + 0^2, counts once */

    uint64_t n_tmp = k;
    seq_t    prod  = 1;        /* product over primes ≡ 1 (mod 4) of (exponent+1) */
    bool     bad   = false;

    /* factorise using the global list of primes */
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= n_tmp; ++i)
    {
        uint64_t p = primes[i];
        if (n_tmp % p != 0) continue;

        size_t exp = 0;
        while (n_tmp % p == 0)
        {
            n_tmp /= p;
            ++exp;
        }

        if (p % 4 == 3)
        {
            if (exp & 1) {  /* odd exponent of 3 mod 4 prime => no representation */
                bad = true;
                break;
            }
            /* even exponent contributes nothing to prod */
        }
        else if (p % 4 == 1)
        {
            prod *= (seq_t)(exp + 1);
        }
        /* p == 2 contributes nothing to prod */
    }

    if (bad) return 0;   /* representation impossible */

    if (n_tmp > 1)   /* remaining prime factor */
    {
        if (n_tmp % 4 == 3)  /* exponent 1, odd */
            return 0;

        if (n_tmp % 4 == 1)
            prod *= 2;      /* exponent 1 => (1+1) */
        /* if n_tmp == 2, nothing changes */
    }

    seq_t r2 = prod * 4;          /* number of ordered sign representations */
    seq_t val = r2 / 8;           /* integer division truncates */

    /* special cases: squares and twice squares */
    seq_t sq   = sqrt_floor((seq_t)k);
    if (sq * sq == (seq_t)k)      /* k = a^2 + 0^2 */
        val++;
    if ((k & 1ULL) == 0)          /* k even: check k = a^2 + a^2  */
    {
        seq_t half = sqrt_floor((seq_t)(k / 2));
        if (half * half == (seq_t)(k / 2))
            val++;
    }

    return val;
}

/* ---- main sequence function ---- */
seq_t
a(idx_t n)
{
    if (n < 1) return 0;      /* offset 1: values before 1 are irrelevant */

    /* ensure we have primes up to n for both iteration and factorisation */
    ensure_primes_upto(n);

    seq_t sum = 0;
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        uint64_t p = primes[i];
        if (p > (uint64_t)n)
            break;
        sum += b_of(p - 1);
    }
    return sum;
}
