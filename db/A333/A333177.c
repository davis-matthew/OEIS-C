/*
 * A333177  a(n) = Sum_{d|n, gcd(d, n/d) = 1} (-1)^omega(n/d) * prime(d).
 * https://oeis.org/A333177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{d|n, gcd(d, n/d) = 1} (-1)^omega(n/d) * prime(d)
 *            where omega(m) is the number of distinct prime factors of m and
 *            prime(k) is the k-th prime (prime(1)=2)
 * Range:     u8 n <= 58, u16 n <= 6546, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9986 (2026-11-01)
 * Helpers:   gcd, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/omega.h"

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
        return (seq_t)0;   /* undefined for n < 1, return 0 as safety */

    seq_t sum = 0;
    idx_t i;

    for (i = 1; i * i <= n; ++i)
    {
        if ((n % i) != 0)
            continue;

        idx_t d = i;
        idx_t e = n / i;

        /* coprime test */
        if (gcd((seq_t)d, (seq_t)e) == (seq_t)1)
        {
            /* contribution from divisor d */
            int omega_e = omega(e);
            seq_t prime_d = get_prime(d);
            if (omega_e & 1)
                sum -= prime_d;
            else
                sum += prime_d;

            /* if d != e, process the pair (e,d) separately */
            if (d != e && gcd((seq_t)e, (seq_t)d) == (seq_t)1)
            {
                int omega_d = omega(d);
                seq_t prime_e = get_prime(e);
                if (omega_d & 1)
                    sum -= prime_e;
                else
                    sum += prime_e;
            }
        }
    }

    return sum;
}
