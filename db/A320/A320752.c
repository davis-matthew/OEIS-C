/*
 * A320752  Primes of the form 5*n^2 - 5*n + 13.
 * https://oeis.org/A320752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 60, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 949 (2026-11-03)
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

/* Return the n-th prime of the form 5*n^2 - 5*n + 13 (1‑based). */
seq_t a(idx_t n)
{
    /* Cache of found primes. */
    static seq_t *primes = NULL;      /* array of primes found */
    static size_t primes_cap = 0;     /* allocated capacity */
    static size_t primes_cnt = 0;     /* number of primes stored */
    static idx_t last_k = 0;          /* largest k already inspected */

    /* Grow the cache if necessary. */
    if (primes_cap < primes_cnt + 1024) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 1024;
        if (new_cap < primes_cnt + 1024)
            new_cap = primes_cnt + 1024;
        seq_t *tmp = mem_resize(seq_t, primes, new_cap);
        if (!tmp)
            abort();
        primes = tmp;
        primes_cap = new_cap;
    }

    /* Fast return if the requested term is already cached. */
    if (n <= primes_cnt)
        return primes[n - 1];

    /* Search for additional primes of the form 5*k^2 - 5*k + 13. */
    idx_t k;
    for (k = last_k + 1; primes_cnt < n; ++k) {
        seq_t kk = (seq_t)k;
        seq_t val = (seq_t)5 * kk * kk - (seq_t)5 * kk + (seq_t)13;
        if (is_prime_big(val))
            primes[primes_cnt++] = val;
    }

    last_k = k - 1;  /* remember the last inspected k */

    return primes[n - 1];
}
