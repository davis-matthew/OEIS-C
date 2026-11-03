/*
 * A244800  Number of moduli m such that (prime(n) mod m) is even, where 1 <=
 *          m < prime(n).
 * https://oeis.org/A244800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of m (1 ≤ m ≤ prime(n)) such that prime(n) mod m
 *            is even
 * Range:     u8 n <= 130, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 967 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    if (n == 0)
        return 0; /* undefined for offset < 1 */

    /* obtain the n‑th prime */
    uint64_t p = (uint64_t)get_prime(n);

    uint64_t count = 0;
    uint64_t m = 1;

    /* parity of the prime */
    int p_even = (p & 1ULL) == 0;

    while (m <= p)
    {
        uint64_t q = p / m;      /* floor(p/m) */
        uint64_t r = p / q;      /* last m with this q */

        if (p_even)
        {
            if (q & 1ULL)        /* q odd → need m even */
            {
                uint64_t evens = (r / 2ULL) - ((m - 1ULL) / 2ULL);
                count += evens;
            }
            else                 /* q even → all m satisfy */
            {
                count += (r - m + 1ULL);
            }
        }
        else
        {
            if (q & 1ULL)        /* q odd → need m odd */
            {
                uint64_t odds = ((r + 1ULL) / 2ULL) - (m / 2ULL);
                count += odds;
            }
            /* q even → none */
        }

        m = r + 1ULL;
    }

    return (seq_t)count;
}
