/*
 * A391928  Irregular table T(n, k) = (k*A002110(n-1)) mod prime(n), n >= 0, k
 *          = 0..prime(n)-1, read by rows, with row 0 = {0} by convention.
 * https://oeis.org/A391928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   a(n) = (k*A062347(n-1)) mod prime(n) where k = a(n) position in
 *            its row * A062347(n) is the primorial of the n-th prime (product
 *            of all primes ≤ prime(n)). * The sequence is read row‑by‑row,
 *            with row 0 = {0} by convention
 * Range:     u8 n <= 6200, u16 n <= 18189, u32 n <= 18189, u64 n <= 18189,
 *            u128 n <= 18189, big any n
 * Validated: standard, pass, 117 terms through n = 8855 (2026-11-01)
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
        return 0;

    /* find the row that contains index n and the prime of that row */
    idx_t cum = 1;           /* cumulative count of terms up to the start of next row */
    idx_t row = 0;
    uint32_t row_prime = 0;

    for (idx_t r = 1; ; ++r)
    {
        uint32_t pr = get_prime(r);
        if (cum + pr > n)
        {
            row = r;
            row_prime = pr;
            break;
        }
        cum += pr;
    }

    /* position of n within its row (0‑based) */
    idx_t k = n - cum;               /* we keep k as idx_t to avoid overflow */
    seq_t k_seq = (seq_t)k;

    /* compute A062347(row-1) modulo row_prime:
     * product of primes 2,3,5,...,prime(row-1) modulo row_prime
     */
    seq_t factor = 1;
    uint32_t mod_prime = (uint32_t)row_prime;
    for (idx_t i = 1; i < row; ++i)   /* primes up to prime(row-1) */
    {
        seq_t pi = (seq_t)get_prime(i);
        factor = (factor * pi) % (seq_t)mod_prime;
    }

    /* final value */
    seq_t result = (k_seq * factor) % (seq_t)mod_prime;
    return result;
}
