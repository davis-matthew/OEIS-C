/*
 * A260928  Number of positive integers k < prime(n)/2 such that k + k' is a
 *          square, where k' is the unique integer among 1, ..., prime(n)-1
 *          such that k*k' == 1 (mod prime(n)).
 * https://oeis.org/A260928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 4712, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9251 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A260928: for the n‑th prime p = prime(n), count the
   number of positive integers k < p/2 such that k + k′ is a perfect
   square.  Here k′ is the unique integer 1 ≤ k′ ≤ p−1 with
   k·k′ ≡ 1 (mod p).  The function performs a direct scan of the
   admissible k's, computing the modular inverse by the extended
   Euclidean algorithm (no memory‑heavy modular exponentiation). */

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

/* Compute the modular inverse of k modulo p (p prime),
   using the extended Euclidean algorithm.  All values are
   safely within 64‑bit signed integers for the primes used
   in the sequence. */
static inline seq_t modinv_uint64(idx_t k, idx_t p)
{
    int64_t a = (int64_t)p;
    int64_t b = (int64_t)k;
    int64_t x0 = 0, x1 = 1;

    while (b != 0) {
        int64_t q = a / b;
        int64_t tmp = a - q * b; a = b; b = tmp;
        tmp = x0 - q * x1;       x0 = x1; x1 = tmp;
    }
    /* a is gcd(p,k); for prime p and 1≤k<p, a must be 1. */
    if (x0 < 0) x0 += (int64_t)p;
    return (seq_t)x0;
}

/* Integer square root for 0 <= n <= 2^64-1. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = 1ULL << 32;           /* high > sqrt(2^64-1) */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (mid > 0 && mid > n / mid)
            high = mid;
        else
            low = mid + 1;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* Retrieve the n‑th prime. */
    seq_t p64 = (seq_t)get_prime(n);
    if (p64 <= 2) return 0;

    idx_t p = (idx_t)p64;
    idx_t kmax = (p - 1) / 2;
    seq_t count = 0;

    for (idx_t k = 1; k <= kmax; ++k) {
        seq_t inv = modinv_uint64(k, p);
        seq_t sum = (seq_t)k + inv;          /* k + k' < 2p <= 2^18 */
        idx_t s = (idx_t)sum;
        idx_t r = isqrt_u64(s);
        if ((r * r) == s)
            ++count;
    }

    return count;
}
