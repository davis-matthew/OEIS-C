/*
 * A350957  Number of ways to write 2*n as 3^i (i >= 0) plus a prime.
 * https://oeis.org/A350957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the OEIS sequence A350957:
   a(n) = number of ways to write 2*n as 3^i (i >= 0) plus a prime. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Helper: prime test for 64‑bit values (used only when p is out of the
   pre‑computed small prime range). */
static bool is_prime_u64(uint64_t x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Simple sieve for primes up to a fixed limit.
       For n up to 10 000 this limit of 20 000 always covers all possible
       prime candidates 2*n – 3^i.  The table is built once. */
    static unsigned char *prime_tbl = NULL;
    static size_t tbl_limit = 0;
    const size_t PRECOMP_LIMIT = 20000;

    if (!prime_tbl) {
        tbl_limit = PRECOMP_LIMIT;
        prime_tbl = (unsigned char*)calloc(tbl_limit + 1, 1);
        if (!prime_tbl) return 0;  /* fallback in case of allocation failure */

        /* Sieve of Eratosthenes */
        for (size_t i = 2; i <= tbl_limit; ++i) prime_tbl[i] = 1;
        for (size_t p = 2; p * p <= tbl_limit; ++p) {
            if (prime_tbl[p]) {
                for (size_t k = p * p; k <= tbl_limit; k += p) {
                    prime_tbl[k] = 0;
                }
            }
        }
    }

    seq_t count = 0;
    seq_t two_n = (seq_t)n * 2;
    seq_t pow3 = 1;   /* 3^i */

    while (pow3 <= two_n) {
        seq_t p = two_n - pow3;
        if (p >= 2) {
            if ((size_t)p <= tbl_limit) {
                if (prime_tbl[(size_t)p]) ++count;
            } else if (is_prime_u64((uint64_t)p)) {
                ++count;
            }
        }
        /* next power of 3 */
        pow3 *= 3;
    }

    return count;
}
