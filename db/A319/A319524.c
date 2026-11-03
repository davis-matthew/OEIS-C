/*
 * A319524  a(n) is the smallest number that belongs simultaneously to the two
 *          arithmetic progressions prime(n) + m*prime(n+1) and prime(n+1) +
 *          m*prime(n+2), m >= 1, n >= 1.
 * https://oeis.org/A319524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = smallest number in the intersection of * the arithmetic
 *            progressions prime(n)+m*prime(n+1) and * prime(n+1)+m*prime(n+2)
 *            for m ≥ 1
 * Range:     u8 n <= 5, u16 n <= 53, u32 n <= 6541, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 64 terms through n = 9528 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Include helper functions – assumed to be present in the same
   framework as provided by the build system. */
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/powmod.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* Undefined for n < 1, return 0 */

    /* Obtain the required primes.  get_prime is 1-indexed. */
    seq_t p_n  = get_prime(n);
    seq_t p_n1 = get_prime(n + 1);
    seq_t p_n2 = get_prime(n + 2);

    /* Base values for the two progressions */
    seq_t base1 = p_n + p_n1;
    seq_t base2 = p_n1 + p_n2;
    seq_t need = (base1 > base2) ? base1 : base2;

    /* CRT setup for coprime moduli: m = p_n1, n = p_n2 */
    seq_t a1 = p_n;          /* k ≡ a1 (mod m) */
    seq_t m  = p_n1;
    seq_t b1 = p_n1;        /* k ≡ b1 (mod n) */
    seq_t n_mod = p_n2;

    /* Compute difference modulo n */
    seq_t diff = (a1 > b1) ? (b1 + n_mod - a1) : (b1 - a1);

    /* Modular inverse of m modulo n using Fermat's little theorem */
    seq_t inv = powmod(m, (uint64_t)(n_mod - 2), n_mod);

    /* CRT solution in [0, m*n_mod) */
    seq_t mult = (diff * inv) % n_mod;
    seq_t x0   = a1 + m * mult;

    seq_t M = m * n_mod;
    if (x0 >= M) x0 -= M;  /* bring into canonical range */

    /* Ensure that the solution lies in the valid range
       where both progressions use m ≥ 1. */
    seq_t result = x0;
    if (result < need) {
        seq_t t = (need - result + M - 1) / M; /* ceil((need-result)/M) */
        result += t * M;
    }

    return result;
}
