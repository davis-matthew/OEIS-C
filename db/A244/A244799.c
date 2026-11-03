/*
 * A244799  Number of moduli m such that (prime(n) mod m) is odd, where 1 <= m
 *          < prime(n).
 * https://oeis.org/A244799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 76 terms through n = 890 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count m such that 1 <= m < prime(n) and (prime(n) mod m) is odd. */
/* Even m always give an odd residue.  For odd m the residue is odd
   iff floor(prime(n)/m) is even. */
#include <stdint.h>
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
    if (n < 1)
        return 0;

    /* Retrieve the nth prime.  get_prime returns a uint32_t which is
       safe for n up to the size of the b-file (primes fit in 32 bits). */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* All even m give odd residues. */
    seq_t even_m = (p - 1) / 2;

    /* Count odd m with floor(p/m) even. */
    seq_t odd_even_k = 0;
    for (uint32_t m = 3; m < p32; m += 2) {
        uint32_t k = p32 / m; /* floor division */
        if ((k & 1u) == 0u)    /* even k */
            odd_even_k++;
    }

    return even_m + odd_even_k;
}
