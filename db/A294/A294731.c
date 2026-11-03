/*
 * A294731  Smallest average of a twin prime pair divisible by the n-th prime,
 *          i.e. A090530(n), divided by 6*prime(n).
 * https://oeis.org/A294731
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A090530(n) / (6 * prime(n)) for n ≥ 3, where A090530(n)
 *            is the smallest average of a twin prime pair divisible by the
 *            n-th prime. The implementation searches successive multiples of
 *            6·prime(n) until the number minus 1 and plus 1 are both prime
 * Range:     u8 n <= 5, u16 n <= 65, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 8632 (2026-11-03)
 * Helpers:   build_primes, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes.h"
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

static idx_t *prime_array = NULL;   /* array of primes, 0‑based */
static size_t prime_count = 0;      /* number of primes stored */

/* Ensure that prime_array contains at least num_primes primes.
   We pre‑allocate a fixed upper bound sufficient for n ≤ 10000. */
static void ensure_primes(idx_t needed)
{
    if (prime_array != NULL && needed <= prime_count)
        return;                     /* already enough */

    /* All required primes will be below 200 000 for n ≤ 10 000. */
    size_t limit = 200000;
    build_primes((idx_t)limit, &prime_array, &prime_count);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 3)                         /* undefined for offset range */
        return 0;

    /* Make sure we have the n-th prime. */
    ensure_primes(n);
    if (n > prime_count)               /* safety check */
        return 0;

    /* nth prime (1‑based) */
    seq_t p = (seq_t)prime_array[n - 1];

    /* Step size: 6·p.  For t = 1, average = step. */
    seq_t step = (seq_t)6 * p;
    seq_t avg  = step;
    seq_t t    = (seq_t)1;

    for (;; ++t, avg += step) {
        if (is_prime_big(avg - 1) && is_prime_big(avg + 1))
            return t;                  /* a(n) equals this t */
    }
}
