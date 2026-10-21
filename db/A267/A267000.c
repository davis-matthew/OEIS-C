/*
 * A267000  a(n) is the smallest m such that A001414(m)=n and ((m=0) mod n)
 *          and m/n is both squarefree and prime to n, or 0 if no such m
 *          exists.
 * https://oeis.org/A267000
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 89, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   _is_banned
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A267000
   a(n) = smallest m such that
          A001414(m)=n ,
          m ≡ 0 (mod n) ,
          m/n is square‑free and coprime to n,
          or 0 if no such m exists.
   Implementation using a per‑term dynamic programme over distinct primes
   with sum equal to n – A001414(n).  The routine is written for all
   supported integer types seq_t and idx_t.  All auxiliary integer
   operations are integer.  The algorithm is O(π(d)·d) per call,
   where d = n – A001414(n) and π(d) is the prime counting function.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/_is_banned.h"

/* ---------- configuration ------------------------------------------------- */
#if !defined(MAX_N)
#  define MAX_N 10000   /* maximum n expected in the tests */
#endif

/* ---------- global prime data -------------------------------------------- */
static bool _initialized = false;
static int  _primes[MAX_N + 1];
static int  _prime_cnt;
static uint64_t _spf[MAX_N + 1];

/* ---------- helper: sieve and smallest prime factor ---------------------- */
static void _init_sieve(void)
{
    int i, j, p;
    for (i = 0; i <= MAX_N; ++i) {
        _spf[i] = (uint64_t)i;
    }
    for (i = 2; i * i <= MAX_N; ++i) {
        if (_spf[i] == (uint64_t)i) {           /* i is prime          */
            for (j = i * i; j <= MAX_N; j += i) {
                if (_spf[j] == (uint64_t)j) {
                    _spf[j] = (uint64_t)i;
                }
            }
        }
    }
    _prime_cnt = 0;
    for (i = 2; i <= MAX_N; ++i) {
        if (_spf[i] == (uint64_t)i) {
            _primes[_prime_cnt++] = i;
        }
    }
}

/* ---------- helper: sum of prime factors with multiplicity ----------------*/
static seq_t _sum_pf(idx_t n)
{
    seq_t total = 0;
    uint64_t x = (uint64_t)n;
    while (x > 1) {
        uint64_t p = _spf[(int)x];
        total += (seq_t)p;
        x /= p;
    }
    return total;
}

/* ---------- main function -------------------------------------------------*/
seq_t a(idx_t n)
{
    /* initialise primes once */
    if (!_initialized) {
        _init_sieve();
        _initialized = true;
    }

    if (n < 2) {
        return 0;
    }

    /* compute sum of prime factors of n */
    seq_t sum_n = _sum_pf(n);

    /* d = n - sum_n */
    if (sum_n > n) {
        return 0;
    }
    idx_t d = (idx_t)(n - sum_n);
    if (d == 0) {                   /* A001414(n) = n      */
        return n;
    }

    /* list of distinct primes dividing n (banned primes for k) */
    int banned[16], banned_cnt = 0;
    {
        idx_t tmp = n;
        while (tmp > 1) {
            int p = _spf[(int)tmp];
            if (! _is_banned(p, banned, banned_cnt)) {
                banned[banned_cnt++] = p;
            }
            while (tmp % (idx_t)p == 0) {
                tmp /= (idx_t)p;
            }
        }
    }

    /* dynamic programme to find minimal product of distinct primes
       with sum d that avoids the banned primes. */
    static seq_t best[MAX_N + 1];
    /* clear best[0..d] */
    for (idx_t i = 0; i <= d; ++i) {
        best[i] = 0;
    }
    best[0] = 1;                     /* empty set */

    for (int pi = 0; pi < _prime_cnt; ++pi) {
        int p = _primes[pi];
        if (p > d) break;
        if (_is_banned(p, banned, banned_cnt)) continue;

        /* 0‑1 knapsack update */
        for (idx_t s = d; s >= (idx_t)p; --s) {
            if (best[s - p] != 0) {
                seq_t candidate = (seq_t)p * best[s - p];
                if (best[s] == 0 || candidate < best[s]) {
                    best[s] = candidate;
                }
            }
        }
    }

    if (best[d] == 0) {
        return 0;          /* no suitable k found */
    }

    /* return m = n * k */
    return (seq_t)n * best[d];
}
