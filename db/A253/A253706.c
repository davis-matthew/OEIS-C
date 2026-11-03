/*
 * A253706  Primes in the 8th-order Fibonacci numbers A079262.
 * https://oeis.org/A253706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = primes in the 8th-order Fibonacci numbers A079262
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_len = 0;
    static seq_t last[8];

    if (!init) {
        /* first eight 8th‑order Fibonacci terms a(0)..a(7) */
        last[0] = 0;
        last[1] = 1;
        last[2] = 1;
        last[3] = 2;
        last[4] = 4;
        last[5] = 8;
        last[6] = 16;
        last[7] = 32;

        /* allocate cache for primes found */
        primes_cap = 128;
        primes = seq_alloc(primes_cap);
        primes_len = 0;

        /* check for primes among these initial terms */
        for (int i = 0; i < 8; ++i) {
            if (is_prime_big(last[i])) {
                if (primes_len >= primes_cap) {
                    size_t new_cap = primes_cap * 2;
                    seq_t *tmp = seq_realloc(primes, primes_cap, new_cap);
                    primes = tmp;
                    primes_cap = new_cap;
                }
                primes[primes_len++] = last[i];
            }
        }
        init = true;
    }

    /* Generate further 8th‑order Fibonacci numbers until we have n primes */
    while (primes_len < n) {
        seq_t sum = 0;
        for (int i = 0; i < 8; ++i) sum += last[i];

        if (is_prime_big(sum)) {
            if (primes_len >= primes_cap) {
                size_t new_cap = primes_cap * 2;
                seq_t *tmp = seq_realloc(primes, primes_cap, new_cap);
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = sum;
        }

        /* slide the window: drop the oldest term, append the new term */
        for (int i = 0; i < 7; ++i)
            last[i] = last[i + 1];
        last[7] = sum;
    }

    return primes[n - 1];
}
