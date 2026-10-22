/*
 * A392264  a(n) is the least prime p such that n + p is the digit reversal of
 *          a prime.
 * https://oeis.org/A392264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 9999, u32 n <= 9999, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime_big, next_prime, rev_decimal
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
#include "../utils/build_primes_upto.h"
#include "../utils/rev_decimal.h"
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"

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

        seq_t rev = rev_decimal(tmp);             /* reverse decimal digits */
        if (is_prime_big(rev))                    /* found minimal p */
            return p;
    }

    /* 2. If not found in the table, continue with larger primes */
    uint64_t next = next_prime(prime_table[prime_count - 1]);

    for (;;) {
        seq_t p = (seq_t)next;

        seq_t tmp = (seq_t)n + p;
        if (tmp % (seq_t)10 != (seq_t)0 && is_prime_big(rev_decimal(tmp)))
            return p;

        next = next_prime(next);
    }
}
