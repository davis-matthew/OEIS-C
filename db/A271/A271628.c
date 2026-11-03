/*
 * A271628  Primes that are the sum of the digits of the numbers from 1 to n,
 *          for some n.
 * https://oeis.org/A271628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the nth prime that equals the cumulative sum of the *
 *            digit sums of numbers from 1 up to some m. That is, * a(n) is
 *            the nth value of `sum_{k=1}^{m} digit_sum(k)` that is prime. *
 *            (OEIS A271628)
 * Range:     u8 n <= 4, u16 n <= 156, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9643 (2026-11-03)
 * Helpers:   digit_sum, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/mem.h"
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
    /* static state for memoisation */
    static seq_t *prime_list = NULL;   /* array of primes found so far */
    static size_t prime_cap  = 0;      /* current vector capacity    */
    static size_t prime_len  = 0;      /* number of primes stored    */
    static idx_t   current_m   = 0;    /* last m processed           */
    static seq_t  current_sum = 0;    /* cumulative sum up to m     */

    if (n == 0) return 0;  /* undefined index – return 0 */

    /* initialise the prime array on the first call */
    if (prime_list == NULL) {
        prime_cap  = 16;
        prime_list = mem_new(seq_t, prime_cap);
        if (!prime_list) return 0;  /* allocation failure – unlikely */
    }

    /* generate primes until we have at least n of them */
    while (prime_len < (size_t)n) {
        /* move to the next integer m */
        ++current_m;

        /* add its digit sum to the running total */
        current_sum += digit_sum((seq_t)current_m);

        /* if the new total is prime, store it */
        if (is_prime_big(current_sum)) {
            if (prime_len == prime_cap) {   /* grow the array */
                prime_cap *= 2;
                seq_t *tmp = mem_resize(seq_t, prime_list, prime_cap);
                if (!tmp) return 0;  /* allocation failure */
                prime_list = tmp;
            }
            prime_list[prime_len++] = current_sum;
        }
    }

    return prime_list[n - 1];
}
