/*
 * A398161  Numbers k such that every exponent in the prime factorizations of
 *          both k and k+1 is either 1 or even.
 * https://oeis.org/A398161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 198, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes a(n) for OEIS A398161: 
   numbers k such that every prime exponent in the factorisation of k 
   and of k+1 is either 1 or even.  The sequence is built by
   iterating over all integers k and selecting those that satisfy the
   property for both k and k+1.  Results are memoised so that multiple
   calls can use previously computed terms.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- helper to test the property on a uint64_t ------------------ */
static inline bool good_uint64(uint64_t n)
{
    if (n <= 1) return true;          /* 1 and 0 are trivially good */

    uint64_t tmp = n;
    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                ++exp;
                tmp /= p;
            }
            if (exp == 1 || (exp & 1) == 0) {
                /* ok */
            } else {
                return false;          /* odd exponent > 1 found */
            }
        }
    }
    if (tmp > 1) {
        /* remaining prime factor has exponent 1 – always ok */
    }
    return true;
}

/* cast seq_t to uint64_t – works for all terms that fit into 64 bits  */
static inline bool good(seq_t x)
{
    return good_uint64((uint64_t)x);
}

/* ---------- memoised sequence generation -------------------------------- */
seq_t a(idx_t n)
{
    /* static storage for already computed terms */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static size_t  len    = 0;
    static idx_t   last   = 0;          /* last integer checked as a k   */

    /* initialise storage */
    if (!cache) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failure */
    }

    /* generate terms until we have at least n of them */
    while (len < n) {
        ++last;                         /* candidate k  */
        if (good((seq_t)last) && good((seq_t)(last + 1))) {
            /* store k */
            if (len == cap) {
                cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;    /* allocation failure */
                cache = tmp;
            }
            cache[len++] = (seq_t)last;
        }
    }

    /* return the requested term */
    return cache[n - 1];
}
