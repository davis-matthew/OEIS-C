/*
 * A363840  a(n) is the number of isomorphism classes of genus 2 hyperelliptic
 *          curves over the finite field of order prime(n).
 * https://oeis.org/A363840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for n = 1, a(1) = 20; for n > 1, a(n) = 2*p^3 + p^2 + 2*p - 2 +
 *            2*[p ≡ 1 (mod 3)] + 8*[p ≡ 1 (mod 5)] + 2*[p = 5] + 2*[p ≡ 1 or
 *            3 (mod 8)] where p = prime(n) (nth prime)
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the helper function get_prime to retrieve the nth prime. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

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
    if (n == 0) return 0;          /* undefined but safe */
    if (n == 1) return (seq_t)20;  /* special case */

    /* Retrieve nth prime (uint32_t returns 0 if n is out of range). */
    uint32_t prime_u = get_prime(n);
    if (prime_u == 0) return 0;    /* safety check */

    seq_t p = (seq_t)prime_u;
    seq_t val = 2 * p * p * p + p * p + 2 * p - 2;

    /* Indicator terms based on congruence conditions. */
    if (prime_u % 3 == 1) val += 2;
    if (prime_u % 5 == 1) val += 8;
    if (prime_u == 5) val += 2;
    if (prime_u % 8 == 1 || prime_u % 8 == 3) val += 2;

    return val;
}
