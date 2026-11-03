/*
 * A392862  a(n) = A143293(n)+14 mod prime(n+1), where A143293 gives the
 *          partial sums of primorials.
 * https://oeis.org/A392862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A143293(n)+14) mod prime(n+1)
 * Range:     u8 n <= 53, u16 n <= 4910, u32 n <= 7979, u64 n <= 12000,
 *            u128 n <= 12000, big any n
 * Validated: standard, pass, 91 terms through n = 9048 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

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

/* Computes a(n) = (sum_{k=0}^n primorial(k) + 14) mod prime(n+1),
   where primorial(k) = product of the first k primes (with primorial(0)=1). */
seq_t a(idx_t n)
{
    /* Prime associated with this n (prime(n+1)) */
    seq_t mod = (seq_t)get_prime(n + 1);

    /* Running sum of primorials modulo mod */
    seq_t sum_mod = 0;

    /* Current primorial value modulo mod (starts with primorial(0)=1) */
    seq_t product = 1;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t prim;
        if (k == 0) {
            prim = 1;
        } else {
            /* Update primorial: multiply by the next prime (k-th prime) */
            product = (product * (seq_t)get_prime(k)) % mod;
            prim = product;
        }
        /* Add to the running sum modulo mod */
        sum_mod = (sum_mod + prim) % mod;
    }

    /* Final result: add 14 and reduce modulo mod */
    return (sum_mod + 14) % mod;
}
