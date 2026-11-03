/*
 * A285693  a(1) = 2; a(n + 1) = smallest prime > a(n) such that a(n + 1) -
 *          a(n) is the product of 8 primes.
 * https://oeis.org/A285693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-03)
 * Helpers:   bigomega_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A285693
   a(1) = 2
   a(n+1) = smallest prime > a(n) such that a(n+1)-a(n) has exactly 8 prime factors (counting multiplicities). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/bigomega_seq.h"
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

static inline bool product_of_8_primes(seq_t diff)
{
    return bigomega_seq(diff) == (seq_t)8;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t computed = 0;   /* number of terms stored in cache */

    if (n > computed)
    {
        seq_t *tmp = mem_resize(seq_t, cache, (size_t)n);
        if (!tmp)
            return (seq_t)0;    /* allocation failure */
        cache = tmp;

        if (computed == 0)
        {
            cache[0] = (seq_t)2;
            computed = 1;
        }

        for (idx_t i = computed + 1; i <= n; ++i)
        {
            seq_t prev = cache[computed - 1];
            seq_t cand = prev + (seq_t)1;
            for (;;)
            {
                while (!is_prime_big(cand))
                    cand = cand + (seq_t)1;
                seq_t diff = cand - prev;
                if (product_of_8_primes(diff))
                    break;
                cand = cand + (seq_t)1;
            }
            cache[computed] = cand;
            ++computed;
        }
    }

    return cache[n - 1];
}
