/*
 * A285813  Let p_1<p_2<... be primes such that 2*n-p_i is prime. a(n) is the
 *          smallest i such that 2*n+(odd part of p_i-1) is prime or a(n)=0 if
 *          there is no such i.
 * https://oeis.org/A285813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest i such that 2*n–p_i is prime and *
 *            2*n+odd_part(p_i–1) is prime, otherwise 0
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

/* --- Dynamic prime sieve: prime_is[x] == true iff x is prime.  --- */
static bool *prime_is = NULL;      /* byte per integer */
static idx_t sieve_limit = 0;      /* highest number currently sieved */

/* Ensure that all integers up to 'limit' are marked for primality. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= sieve_limit) return;           /* already sufficient */

    free(prime_is);
    prime_is = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!prime_is) return;                      /* memory failure */

    for (idx_t i = 0; i <= limit; ++i) prime_is[i] = true;
    prime_is[0] = prime_is[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (prime_is[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                prime_is[j] = false;
        }
    }
    sieve_limit = limit;
}

/* Compute the odd part of an integer: remove all factors of 2. */
static seq_t odd_part_uint64(uint64_t x)
{
    seq_t r = (seq_t)x;
    while ((r & 1) == 0) r >>= 1;
    return r;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined below offset */

    idx_t two_n = n + n;                 /* 2*n */
    ensure_primes_upto(two_n);           /* need prime test for 2*n - p */

    /* i counts indices in the filtered subsequence of primes where
     * 2*n - p is prime. It starts at 1. */
    seq_t i = 1;

    for (idx_t p = 2; p < two_n; ++p) {
        if (!prime_is[p]) continue;      /* p must be prime */

        idx_t q = two_n - p;
        if (q < 2 || !prime_is[q]) continue;   /* 2*n-p must be prime */

        seq_t odd = odd_part_uint64(p - 1);
        seq_t candidate = (seq_t)two_n + odd;

        if (is_prime_big(candidate))
            return i;                     /* first suitable i found */

        ++i;                              /* advance filtered index */
    }
    return 0;                              /* no suitable prime */
}
