/*
 * A319230  Triple prime Fibonacci numbers: Fibonacci numbers f such that (1)
 *          f is prime, (2) f's index is prime, and (3) the sum of f's digits
 *          is prime.
 * https://oeis.org/A319230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth Fibonacci number f such that f is prime, the
 *            index of f is prime, and the sum of the decimal digits of f is
 *            prime. The function *a* searches successive Fibonacci indices
 *            and caches each qualifying Fibonacci number
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-03)
 * Helpers:   digit_sum, is_prime, is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime.h"
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

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

/* Cache of found numbers */
static seq_t *cache = NULL;
static size_t cache_cnt = 0;
static size_t cache_cap = 0;
static idx_t   next_index = 3; /* next Fibonacci index to examine */

/* Grow the cache to at least *needed* entries */
static void grow_cache(size_t needed)
{
    if (cache_cnt >= needed) return;
    if (cache_cap == 0) cache_cap = 4;
    while (cache_cap < needed) cache_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
    if (!tmp) abort();           /* unsafe but acceptable in this context */
    cache = tmp;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* Undefined for n==0, return 0 as placeholder */

    /* Ensure cache can hold requested term */
    grow_cache(n);

    /* Expand cache until we have at least n elements */
    while (cache_cnt < (size_t)n)
    {
        /* Check prime index */
        if (is_prime(next_index))
        {
            seq_t f = fib(next_index);

            /* Quick check: small numbers may fit in uint64_t but fib may be large */
            if (is_prime_big(f))
            {
                seq_t sum_seq = digit_sum(f);
                /* sum of digits is guaranteed to fit in 64 bits */
                uint64_t sum = (uint64_t)sum_seq;
                if (is_prime_seq(sum))
                {
                    /* Store qualified Fibonacci number */
                    cache[cache_cnt++] = f;
                }
            }
        }
        ++next_index;
    }

    return cache[n - 1];
}
