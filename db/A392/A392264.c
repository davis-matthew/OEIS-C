/*
 * A392264  a(n) is the least prime p such that n + p is the digit reversal of
 *          a prime.
 * https://oeis.org/A392264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 101 terms through n = 9771 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, next_prime, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): least prime p such that n+p is the digit reversal of a prime,
 *   with trailing zeros in n+p disallowed.
 *   Search over pre‑computed primes and extend with next_prime() if needed. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility headers */
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/next_prime.h"
#include "../utils/reverse_digits.h"
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
    /* Lazily build a prime table once */
    static bool initialized = false;
    static uint64_t *prime_table = NULL;
    static size_t prime_count = 0;
    static const idx_t PRIME_LIMIT = 2000000;   /* ample for n up to ~10000 */

    if (!initialized) {
        /* allocate and fill from the utility function */
        build_primes_upto(PRIME_LIMIT, &prime_table, &prime_count);
        initialized = true;
    }

    /* 1. Search primes up to the table */
    for (size_t idx = 0; idx < prime_count; ++idx) {
        uint64_t p64 = prime_table[idx];          /* prime as 64‑bit integer */
        seq_t  p = (seq_t)p64;                    /* cast to seq_t */

        /* skip n+p ending in 0 (trailing zero disallowed) */
        seq_t tmp = (seq_t)n + p;
        if (tmp % (seq_t)10 == (seq_t)0) continue;

        seq_t rev = reverse_digits(tmp);             /* reverse decimal digits */
        if (is_prime_big(rev))                    /* found minimal p */
            return p;
    }

    /* 2. If not found in the table, continue with larger primes */
    uint64_t next = next_prime(prime_table[prime_count - 1]);

    for (;;) {
        seq_t p = (seq_t)next;

        seq_t tmp = (seq_t)n + p;
        if (tmp % (seq_t)10 != (seq_t)0 && is_prime_big(reverse_digits(tmp)))
            return p;

        next = next_prime(next);
    }
}
