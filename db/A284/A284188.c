/*
 * A284188  a(1)=2; thereafter a(n+1) = a(n)+i if a(n) is a prime and
 *          a(1),...,a(n) contains i primes, or a(n+1) = a(n)-i if a(n) is
 *          composite and a(1),...,a(n) contains i primes.
 * https://oeis.org/A284188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2; for n≥1 let p_n be the number of primes among
 *            a(1),...,a(n). Then a(n+1)=a(n)+p_n if a(n) is prime,
 *            a(n+1)=a(n)-p_n if a(n) is composite
 * Range:     u8 n <= 60, u16 n <= 1868, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 79 terms through n = 9274 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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
    static seq_t *vals   = NULL;   /* a(1), a(2), ... */
    static seq_t *primes = NULL;   /* number of primes up to that term */
    static size_t   cap   = 0;     /* allocated capacity of the arrays    */
    static idx_t    len   = 0;     /* number of terms already computed    */

    if (n == 0) return (seq_t)0;    /* offset is 1; n=0 is outside the sequence */

    /* Initialise the cache on the first call. */
    if (len == 0) {
        size_t init = 16;
        vals   = mem_new(seq_t, init);
        primes = mem_new(seq_t, init);
        vals[0]   = (seq_t)2;  /* a(1) = 2 */
        primes[0] = (seq_t)1;  /* one prime seen so far          */
        len   = 1;
        cap   = init;
    }

    /* Grow the arrays if the requested index is beyond current capacity. */
    size_t need = (size_t)n;
    if (cap < need) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        vals   = mem_resize(seq_t, vals, new_cap);
        primes = mem_resize(seq_t, primes, new_cap);
        cap = new_cap;
    }

    /* Compute terms up to the requested index. */
    for (idx_t i = len; i < n; ++i) {
        seq_t last   = vals[i-1];
        bool  lastp  = is_prime_big(last);
        seq_t pcount = primes[i-1];                     /* primes among a(1)…a(i) */

        seq_t next = lastp ? last + pcount : last - pcount;
        vals[i]   = next;

        bool nextp = is_prime_big(next);
        primes[i] = primes[i-1] + (nextp ? (seq_t)1 : (seq_t)0);
    }

    len = n;
    return vals[n-1];
}
