/*
 * A383240  Rectangular array read by antidiagonals where row n contains the
 *          numbers of the form prime(n)*m^2, where prime(n) does not divide
 *          m.
 * https://oeis.org/A383240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A383240: a(n) = p_i * m^2 where (i,j) is the n‑th term read by
   antidiagonals of the array whose row i contains the numbers
   p_i * m^2 (p_i the i‑th prime) for m running over the positive
   integers not divisible by p_i. j is the j‑th such m. */

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
    idx_t low = 1;
    idx_t high = 2;

    /* Double high until the triangular number is at least n */
    while ( ((unsigned __int128)high * (high - 1)) / 2 < n )
        high <<= 1;

    /* Binary search for the smallest s with s*(s-1)/2 >= n */
    while (low < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        idx_t tri = (unsigned __int128)mid * (mid - 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;

    /* Number of terms before this diagonal */
    idx_t prev = (unsigned __int128)(s - 1) * (s - 2) / 2;

    /* Offset within the diagonal (1‑based).  i is the row, j the column. */
    idx_t offset = n - prev;
    idx_t i = offset;
    idx_t j = s - offset;

    /* Prime corresponding to the row */
    uint32_t p = get_prime(i);          /* 1‑based index */

    /* j‑th integer not divisible by p:
         m = j + floor((j-1)/(p-1))   */
    idx_t m = j + (j - 1) / (idx_t)(p - 1);

    /* Result: p * m^2 */
    unsigned __int128 prod = (unsigned __int128)p * m * m;
    return (seq_t)prod;
}
