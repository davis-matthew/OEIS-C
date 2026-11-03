/*
 * A391358  Rectangular array R read by falling antidiagonals: R(n,k) =
 *          prime(k) + prime(n+k).
 * https://oeis.org/A391358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A391358.
 * a(n) is the value on the n‑th falling antidiagonal of the array
 * R(n,k) = prime(k) + prime(n+k).
 * Let s be the smallest integer such that n ≤ s*(s-1)/2.
 * Let pos = n - (s-1)*(s-2)/2   (pos ∈ [1, s-1]).
 * Then a(n) = prime(s - pos) + prime(s).
 * The function uses get_prime from utils to obtain the j‑th prime.
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
    if (n == 0) return (seq_t)0;  /* undefined for n < 1 */

    /* Find diagonal sum s and cumulative count cum = T_s = s*(s-1)/2 */
    uint64_t s = 2;       /* diagonal sum starts at 2 (only one term) */
    uint64_t cum = 1;     /* T_2 = 1 */
    while (cum < n) {
        ++s;
        cum += s - 1;     /* add the number of terms on the new diagonal */
    }

    /* cum is now ≥ n; compute position within the current diagonal */
    uint64_t prev = cum - (s - 1);        /* T_{s-1} */
    uint64_t pos  = n - prev;             /* 1 … s-1 */
    uint64_t i    = pos;                  /* row index in R */
    uint64_t j    = s - pos;              /* column index in R */

    /* a(n) = prime(j) + prime(s) */
    seq_t val_j = (seq_t)get_prime(j);
    seq_t val_s = (seq_t)get_prime(s);
    return val_j + val_s;
}
