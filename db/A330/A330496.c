/*
 * A330496  Squared area of quadrilateral with sides prime(n), prime(n+1),
 *          prime(n+2), prime(n+3) of odd primes configured as a cyclic
 *          quadrilateral. Sequence index starts at n=2 to omit the even
 *          prime.
 * https://oeis.org/A330496
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((p_n+p_{n+1}+p_{n+2}-p_{n+3}) *
 *            (p_n+p_{n+1}-p_{n+2}+p_{n+3}) * (p_n-p_{n+1}+p_{n+2}+p_{n+3}) *
 *            (-p_n+p_{n+1}+p_{n+2}+p_{n+3}))/16 where p_k denotes the k‑th
 *            prime (the first prime is 2). The sequence is defined for n ≥ 2;
 *            for n < 2 the function returns 0
 * Range:     u8 none, u16 n <= 2, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 29 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* Accept only valid indices.  The tests never invoke
       n < 2, but we guard against it. */
    if (n < 2)
        return 0;

    /* Retrieve the four consecutive primes.  get_prime returns
       an unsigned 32‑bit value; we cast to seq_t for all subsequent
       arithmetic.  */
    seq_t a1 = (seq_t)get_prime(n);
    seq_t a2 = (seq_t)get_prime(n + 1);
    seq_t a3 = (seq_t)get_prime(n + 2);
    seq_t a4 = (seq_t)get_prime(n + 3);

    /* Compute the four terms of Brahmagupta's formula.  All
       intermediate results remain non‑negative, so no unsigned
       underflow occurs.  */
    seq_t t1 = a1 + a2 + a3 - a4;
    seq_t t2 = a1 + a2 - a3 + a4;
    seq_t t3 = a1 - a2 + a3 + a4;
    seq_t t4 = a2 + a3 + a4 - a1;

    /* Compute the product and divide by 16.  The division
       is exact because the product is always divisible by
       16, as proven in the OEIS entry.  */
    seq_t prod = t1 * t2;
    prod *= t3;
    prod *= t4;
    prod /= 16;

    return prod;
}
