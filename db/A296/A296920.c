/*
 * A296920  Rational primes that decompose in the quadratic field
 *          Q(sqrt(-11)).
 * https://oeis.org/A296920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime p such that p mod 22 ∈ {1,3,5,9,15}
 * Range:     u8 n <= 24, u16 n <= 3253, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9403 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
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

/* Return true if x ≡ 1,3,5,9,15 (mod 22). */
static bool good_residue(uint64_t x)
{
    uint64_t r = x % 22ULL;
    return r == 1ULL || r == 3ULL || r == 5ULL || r == 9ULL || r == 15ULL;
}

/* Cached primes that satisfy the residue condition. */
static uint64_t *primes = NULL;
static size_t primes_cap = 0;   /* allocated capacity in elements */
static size_t primes_len = 0;   /* number of primes stored */

/* Last integer that was tested (for linear search). */
static uint64_t last_checked = 2;  /* starts at 2; next checked will be 3 */

/* Grow the cached array when needed. */
static void grow_cache(size_t need)
{
    if (need <= primes_cap)
        return;
    size_t new_cap = (need > 8) ? (need * 2) : 8;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp)                      /* allocation failure */
        abort();
    primes = tmp;
    primes_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    assert(n >= 1);  /* OEIS offset is 1 */

    /* Expand the cache until it contains at least n entries. */
    while (primes_len < (size_t)n)
    {
        /* Move to the next candidate. */
        last_checked++;
        if ((last_checked & 1ULL) == 0ULL)   /* skip evens */
            continue;

        /* Test primality. */
        if (!is_prime_big((seq_t)last_checked))
            continue;

        /* Test residue condition. */
        if (!good_residue(last_checked))
            continue;

        /* Cache the prime. */
        if (primes_len == primes_cap)
            grow_cache(primes_len + 1);
        primes[primes_len++] = last_checked;
    }

    /* Return the n‑th term (1‑based). */
    return (seq_t)primes[n-1];
}
