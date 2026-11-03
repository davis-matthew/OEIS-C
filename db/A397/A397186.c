/*
 * A397186  Array read by ascending antidiagonals: A(n,k) = ((k + 1)^prime(n)
 *          - k^prime(n) - 1)/prime(n).
 * https://oeis.org/A397186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 38, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A397186 – array read by ascending antidiagonals.  
 * For each pair (k,n) with k,n≥1 define  
 *   A(k,n) = ((k+1)^prime(n) - k^prime(n) - 1) / prime(n)  
 * where prime(m) is the m‑th prime.  
 * The sequence a(n) lists the values of A(k,n) in order of increasing
 * sums k+n, i.e. first (1,1), then (1,2),(2,1), then (1,3),(2,2),(3,1),
 * etc.  
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"

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

/* Return the n-th term of sequence A397186 (n ≥ 1).  */
seq_t a(idx_t n)
{
    /* Find the smallest s such that (s-1)*s/2 ≥ n.  */
    idx_t low = 2, high = 2;
    while (((unsigned __int128)high * (high - 1)) / 2 < n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 val = ((unsigned __int128)mid * (mid - 1)) / 2;
        if (val >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t s = low;                           /* current antidiagonal sum */

    /* Number of entries before this antidiagonal. */
    idx_t total_prev = ((unsigned __int128)(s - 2) * (s - 1)) / 2;

    /* Row (k) and column (n) indices in A(k,n).  */
    idx_t k = n - total_prev;                /* 1 ≤ k ≤ s-1 */
    idx_t nidx = s - k;                      /* 1 ≤ nidx ≤ s-1 */

    /* Compute the required prime and powers.  */
    uint32_t p = get_prime(nidx);            /* p = prime(nidx) */

    seq_t base_k = (seq_t)k;
    seq_t base_k1 = (seq_t)(k + 1);

    seq_t pow_k1 = ipow_i32(base_k1, (int)p); /* (k+1)^p */
    seq_t pow_k  = ipow_i32(base_k,  (int)p); /* k^p */

    seq_t num = pow_k1;
    num -= pow_k;
    num -= 1;                                /* numerator */

    return num / (seq_t)p;                   /* integer division */
}
