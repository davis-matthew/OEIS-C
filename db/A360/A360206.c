/*
 * A360206  Triangular array T(m,n) read by antidiagonals: T(m,n) = prime(m+n)
 *          - prime(m) - prime(n).
 * https://oeis.org/A360206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   T(m,n) = prime(m+n) - prime(m) - prime(n) and a(n) is obtained
 *            by reading the triangular array T(m,n) (1‑based, 1 ≤ n ≤ m)
 *            row‑by‑row. For a given index n, let m be the smallest integer
 *            with m(m+1)/2 ≥ n. Then ncol = n - (m‑1)m/2 gives the column
 *            (1‑based) of that entry. The function returns a(n)
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static idx_t triangular(idx_t m)
{
    /* returns m*(m+1)/2 using 128‑bit intermediate to avoid overflow */
    return (idx_t)(((__int128)m * (m + 1)) / 2);
}

static idx_t row_of(idx_t n)
{
    /* binary search for the smallest m such that m(m+1)/2 ≥ n */
    idx_t low = 1, high = 1;
    while ((__int128)high * (high + 1) / 2 < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) /* offset is 1; out of range is harmless */
        return (seq_t)0;

    /* find row m and column k (1‑based) for a(n) */
    idx_t m = row_of(n);
    idx_t prev = triangular(m - 1);
    idx_t k = n - prev;                 /* column index in this row */

    /* lookup primes by index (primes are 1‑based) */
    seq_t p_total = (seq_t)get_prime(m + k);
    seq_t p_m     = (seq_t)get_prime(m);
    seq_t p_k     = (seq_t)get_prime(k);

    return p_total - p_m - p_k;
}
