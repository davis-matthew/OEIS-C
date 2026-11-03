/*
 * A241817  Semiprimes sp such that sp-3 is prime.
 * https://oeis.org/A241817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 974, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Validated: standard, pass, 69 terms through n = 2122 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Semantic description:
   a(n) = 2p where p is the n-th prime such that 2p-3 is also prime. */

/* Includes */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
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

/* Cached data */
static seq_t   *terms        = NULL;   /* array of 2p values          */
static size_t   terms_len    = 0;       /* number of computed terms   */
static size_t   terms_cap    = 0;       /* current array capacity     */
static uint64_t last_prime   = 1;       /* last prime examined        */

/* Main function */
seq_t a(idx_t n)
{
    /* n == 0 is outside the defined offset */
    if (n == 0U) return 0U;

    /* Ensure we have enough terms for all requested indices */
    while (terms_len < (size_t)n)
    {
        /* Find next prime after last_prime */
        uint64_t p = next_prime(last_prime);
        last_prime = p;            /* remember for next call */

        /* Skip the trivial prime 2 (since 2*2-3 = 1 is not prime) */
        if (p < 2ULL) continue;

        /* sp = 2p   (always even and semiprime) */
        seq_t sp = (seq_t)p * 2U;

        /* Test whether sp-3 is prime */
        seq_t test = sp - 3U;
        if (test > 1U && is_prime_big(test))
        {
            /* Grow terms array if necessary */
            if (terms_len >= terms_cap)
            {
                size_t new_cap = terms_cap ? terms_cap << 1 : 16U;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                if (!tmp)           /* allocation failed */
                {
                    mem_free(terms);
                    terms = NULL;
                    terms_len = terms_cap = 0U;
                    return 0U;    /* safe failure */
                }
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = sp;
        }
    }

    return terms[n - 1U];
}
