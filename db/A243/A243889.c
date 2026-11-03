/*
 * A243889  Primes of the form 2*n^2+30*n+13.
 * https://oeis.org/A243889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 18, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 53 terms through n = 994 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence of primes of the form 2*n^2+30*n+13.
   The function tests integer values m, evaluates 2*m*m+30*m+13,
   keeps the primes, and returns the nth such prime. */
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

/* External types defined by the build system */
typedef seq_t seq_t;
typedef idx_t idx_t;

seq_t a(idx_t n)
{
    /* Handle invalid index */
    if (n == 0)
        return 0;

    /* Memoization state */
    static seq_t *primes   = NULL;   /* stored primes */
    static size_t primes_cap = 0;    /* allocated capacity */
    static idx_t count = 0;          /* number of primes stored (1‑based) */
    static idx_t last_m = 11;        /* last m examined (next starts at 12) */

    /* If the requested term is already known, return it */
    if ((size_t)n <= count)
        return primes[n - 1];

    /* Ensure sufficient storage */
    if (primes_cap < (size_t)n) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        if (new_cap < (size_t)n)
            new_cap = n;
        seq_t *new_ptr = mem_resize(seq_t, primes, new_cap);
        primes = new_ptr;
        primes_cap = new_cap;
    }

    /* Generate primes of the quadratic until the requested index is reached */
    while (count < n) {
        ++last_m;                     /* m starts from 12 */
        seq_t val = (seq_t)2 * last_m * last_m
                    + (seq_t)30 * last_m + (seq_t)13;
        if (is_prime_big(val)) {
            primes[count++] = val;
        }
    }

    return primes[n - 1];
}
