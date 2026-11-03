/*
 * A373844  Triangle read by rows: T(n,k) = A276086(1 + A002110(n) +
 *          A002110(k)), 1 <= k <= n, where A276086 is the primorial base
 *          exp-function.
 * https://oeis.org/A373844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 2 * p_{r+1} * p_{k+1} where (r,k) is the position of n in
 *            the triangular indexing: r = smallest integer such that
 *            r*(r+1)/2 >= n k = n - r*(r-1)/2 p_i denotes the i-th prime (p_1
 *            = 2, p_2 = 3, …)
 * Range:     u8 n <= 13, u16 n <= 899, u32 n <= 10585, u64 n <= 10585,
 *            u128 n <= 10585, big any n
 * Validated: standard, pass, 82 terms through n = 9807 (2026-11-01)
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
    /* Find row r via binary search on triangular numbers */
    idx_t low = 1, high = n, r;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        // use 128‑bit to avoid overflow
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    r = low;                         /* 1‑based row index */

    /* Start index of the row (triangular number preceding this row) */
    __int128 row_start = (__int128)r * (r - 1) / 2 + 1;

    /* Column index k */
    idx_t k = n - (idx_t)row_start + 1;   /* 1‑based column index */

    /* Get needed primes (1‑based indices) */
    seq_t prime_r = (seq_t)get_prime(r + 1);   /* row prime index r+1 */
    seq_t prime_k = (seq_t)get_prime(k + 1);   /* column prime index k+1 */

    /* Return the value 2 * prime_r * prime_k */
    return (seq_t)2 * prime_r * prime_k;
}
