/*
 * A302721  Square array T(n, k) read by antidiagonals upwards, n > 0 and k >
 *          0: T(n, k) is the distance from n to the nearest prime(k)-smooth
 *          number (where prime(k) denotes the k-th prime number).
 * https://oeis.org/A302721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,k) where (i,k) are the coordinates of the n-th term
 *            when the square array T(n,k) is read by antidiagonals upwards.
 *            That is, for the d-th antidiagonal (d ≥ 1) the entries are
 *            (d,1),(d-1,2),…,(1,d)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   build_primes_upto, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/isqrt.h"

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

/* Prime table used for smoothness testing (up to 20000 is enough
   for all n up to 10 000). */
static uint64_t *smooth_primes = NULL;
static size_t   smooth_prime_cnt = 0;
static bool     primes_built     = false;

/* One-time construction of the prime list. */
static void build_smooth_primes(void)
{
    if (primes_built) return;
    idx_t limit = 20000;                     /* > prime(200)  */
    build_primes_upto(limit, &smooth_primes, &smooth_prime_cnt);
    primes_built = true;
}

/* Test whether x is p‑smooth: all prime factors ≤ p. */
static bool is_smooth(uint64_t x, uint64_t p)
{
    if (x <= 1) return true;                /* 0 and 1 are trivially smooth */

    for (size_t idx = 0; idx < smooth_prime_cnt &&
                         smooth_primes[idx] <= p; ++idx)
    {
        uint64_t q = smooth_primes[idx];
        /* Stop if q^2 > x: remaining factor (if any) is prime. */
        if (q > x / q) break;               /* avoid overflow */

        while (x % q == 0) x /= q;
        if (x == 1) return true;
    }

    /* Remaining x (if >1) is a prime that was not divided out. */
    return x == 1 || x <= p;
}

/* Distance from n to the nearest p‑smooth number. */
static uint64_t smooth_dist(uint64_t n, uint64_t p)
{
    if (is_smooth(n, p)) return 0;
    for (uint64_t d = 1; ; ++d) {
        if (n >= d && is_smooth(n - d, p)) return d;
        if (is_smooth(n + d, p))           return d;
    }
}

/* Convert a linear 1‑based index into antidiagonal coordinates
   (row i, column k).  The d-th antidiagonal contains d terms:
   (d,1),(d-1,2),…,(1,d).  For the i‑th term on that diagonal
   (i = 1 gives (d,1)), the coordinates are:
       row = d - i + 1,   col = i.                     */
static void antidiagonal_coords(idx_t n, idx_t *i, idx_t *k)
{
    /* Find the diagonal d with d*(d+1)/2 ≥ n. */
    idx_t t = isqrt(8 * n + 1);
    if (t * t < 8 * n + 1) ++t;          /* ceil(sqrt(8n+1))     */
    idx_t d = (t - 1) / 2;
    if (d * (d + 1) / 2 < n) ++d;        /* adjust if necessary */

    /* Position within this diagonal (1 … d). */
    idx_t offset = n - d * (d - 1) / 2;  /* 1‑based offset   */

    *i = d - offset + 1;  /* row index   */
    *k = offset;          /* column index */
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety, sequence is 1‑based */

    idx_t row, col;
    antidiagonal_coords(n, &row, &col);

    build_smooth_primes();          /* ensure prime list is ready */

    /* k-th prime; get_prime uses 1‑based indexing. */
    uint32_t pk = get_prime(col);

    uint64_t dist = smooth_dist((uint64_t)row, (uint64_t)pk);

    return (seq_t)dist;
}
