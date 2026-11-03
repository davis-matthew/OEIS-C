/*
 * A263978  Least prime p such that n^2 + p^2 is prime, or 0 if none.
 * https://oeis.org/A263978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that n^2 + p^2 is prime, or 0 if none
 * Range:     u8 n <= 15, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    static uint64_t *primes = NULL;   /* primes array */
    static size_t primes_cnt = 0;     /* number of primes */

    /* initialise the prime list once */
    if (!primes)
    {
        size_t cnt;
        /* 2 M is more than enough for n≤10 000 expected workload */
        build_primes_upto(2000000, &primes, &cnt);
        primes_cnt = cnt;
    }

    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;

    /* Odd n: only p=2 can give a prime; test it */
    if (n & 1ULL)
    {
        seq_t m = n2 + (seq_t)4;   /* n^2 + 2^2 */
        return is_prime_big(m) ? (seq_t)2 : (seq_t)0;
    }

    /* Even n: search primes in increasing order (skip 2) */
    for (size_t i = 1; i < primes_cnt; ++i)   /* primes[0]==2 is skipped */
    {
        uint64_t p  = primes[i];
        seq_t mp    = n2 + (seq_t)p * (seq_t)p;
        if (is_prime_big(mp))
            return (seq_t)p;
    }

    /* If the pregenerated list exhausted, extend search on the fly */
    uint64_t candidate = (primes_cnt ? primes[primes_cnt-1] : 3) + 2;
    if (candidate <= 3) candidate = 5;      /* ensure odd, >2 */
    while (true)
    {
        if (is_prime_big(candidate))
        {
            seq_t mp = n2 + (seq_t)candidate * (seq_t)candidate;
            if (is_prime_big(mp))
                return (seq_t)candidate;
        }
        candidate += 2;   /* only odd numbers are prime candidates */
    }
}
