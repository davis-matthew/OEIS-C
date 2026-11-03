/*
 * A393608  Rectangular array R read by descending antidiagonals: R(n,k) =
 *          prime(k) mod n, for n>=2.
 * https://oeis.org/A393608
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the value of the rectangular array * R(n,k) = prime(k)
 *            mod n, read by ascending n on each * antidiagonal (s = n + k).
 *            This function computes that * value directly using integer
 *            arithmetic only
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 92 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
#include <stdint.h>

seq_t a(idx_t n) {
    /* offset in the sequence (a(2) has offset 0) */
    idx_t i = n - 2;

    /* Find minimal a >= 1 such that a*(a+1) > 2*i.
       Here a = s - 2, where s is the diagonal index.     */
    idx_t two_i = i << 1;          /* 2*i  (safe for the intended n range)  */
    idx_t a_val = 1;
    while (a_val * (a_val + 1) <= two_i) {
        ++a_val;
    }

    /* elements before this diagonal:
       T_prev = (a_val-1)*a_val/2  */
    idx_t t_prev = (a_val - 1) * a_val / 2;
    idx_t offset = i - t_prev;          /* position within the diagonal, 0‑based */

    /* Convert diagonal position to (n,k) */
    idx_t n_val = offset + 2;   /* n increases from 2 upward along the diagonal */
    idx_t k_val = a_val - offset;  /* k = s - n = (a_val+2) - (offset+2) */

    /* Get the k‑th prime and take its remainder modulo n */
    uint32_t prime = get_prime(k_val);  /* returns the k-th prime (1‑based) */

    return (seq_t)prime % (seq_t)n_val;
}
