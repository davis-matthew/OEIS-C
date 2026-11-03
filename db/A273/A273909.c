/*
 * A273909  Let p = prime(n) and q = prime(n+1), then a(n) = p*q - p^2 - 2*q.
 * https://oeis.org/A273909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = prime(n) * prime(n+1) - prime(n)^2 - 2 * prime(n+1) =
 *            (g-2)*p - 2*g, * where p = prime(n), q = prime(n+1), g = q - p
 * Range:     i8 n <= 14, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: this code uses the utility function get_prime(k) to retrieve the k‑th prime. */

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

extern seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0 (behaviour below offset not important). */
    if (n < 1u) return 0;

    /* Retrieve the required primes.  get_prime returns uint32_t, which is safe for n up to 10000. */
    uint32_t p_u = get_prime(n);
    uint32_t q_u = get_prime(n + 1);

    /* Operate in seq_t to avoid overflow on types such as int64_t or __int128. */
    seq_t p = (seq_t)p_u;
    seq_t q = (seq_t)q_u;
    seq_t g = q - p;                  /* the prime gap */
    seq_t result = (g - 2) * p - 2 * g;
    return result;
}
