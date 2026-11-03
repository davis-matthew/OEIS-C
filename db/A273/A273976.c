/*
 * A273976  Primes of the form: Sum_{j=0..k} (k-j)^j.
 * https://oeis.org/A273976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   ipow, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A273976: a(n) is the n‑th prime of the form
 *   S(k) = ∑_{j=0..k} (k−j)^j ,  k ≥ 2. */

/* Standard headers */
#include <stdlib.h>
#include <stdbool.h>

/* Utility functions supplied by the build system */
#include <stdint.h>
#include "../utils/ipow.h"
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
    if (n == 0) return 0;

    /* Cache of primes already found and the next k to test. */
    static seq_t *primes = NULL;      /* array of found primes      */
    static size_t primes_len = 0;     /* number of primes stored    */
    static size_t primes_cap = 0;     /* allocated capacity         */
    static idx_t next_k = 1;          /* next k to test (will become 2) */

    /* Generate primes until we have at least n of them. */
    while (primes_len < n) {
        ++next_k;  /* k = 2, 3, 4, … */

        /* Compute S(k) = ∑_{i=0..k} i^{k-i}.  The i = 0 term is 0. */
        seq_t sum = 0;
        for (idx_t i = 1; i <= next_k; ++i) {
            idx_t exp = next_k - i;          /* non‑negative exponent */
            seq_t pow = ipow((seq_t)i, (seq_t)exp);
            sum += pow;
        }

        /* If the sum is prime, store it in the cache. */
        if (is_prime_big(sum)) {
            if (primes_len >= primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 4;
                seq_t *tmp = mem_resize(seq_t, primes, new_cap);
                if (!tmp) abort();          /* allocation failure */
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = sum;
        }
    }

    /* Return the requested term (1‑based index). */
    return primes[n - 1];
}
