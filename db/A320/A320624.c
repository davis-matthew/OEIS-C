/*
 * A320624  a(n) = (A006134((prime(n)-1)/2) - Legendre(prime(n), 3))/prime(n).
 * https://oeis.org/A320624
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A006134((prime(n)-1)/2) - Legendre(prime(n),3)) /
 *            prime(n) where A006134(m) = sum_{i=0}^m binom(2i,i)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 21 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

seq_t a(idx_t n)
{
    if (n < 2)     /* offset is 2; behavior below is irrelevant */
        return (seq_t)0;

    /* get the nth prime (prime(1)=2, prime(2)=3, ...) */
    uint32_t p = get_prime(n);
    /* k = (p-1)/2 */
    idx_t k = (p - 1) / 2;

    /* compute S = sum_{i=0}^k binom(2*i,i)
       using unsigned __int128 to avoid overflow up to the first case
       where the result still fits in seq_t */
    unsigned __int128 sum = 0;
    unsigned __int128 binom = 1;   /* binom(0,0) = 1 */
    for (idx_t i = 0; i <= k; ++i) {
        if (i > 0)
            binom = (binom * (unsigned __int128)(4 * i - 2)) / (unsigned __int128)i;
        sum += binom;
    }

    /* Legendre symbol (p|3) via Jacobi symbol */
    int leg = jacobi_symbol(p, 3);

    /* numerator = S - leg */
    signed __int128 num = (signed __int128)sum - leg;
    signed __int128 div = num / (signed __int128)p;

    return (seq_t)div;
}
