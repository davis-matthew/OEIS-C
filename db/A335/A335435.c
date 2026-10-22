/*
 * A335435  Triangle read by rows: T(n,k) is the least degree of a polynomial
 *          interpolating (i, prime(i)) for i from k to n.
 * https://oeis.org/A335435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8391, u16 n <= 8391, u32 n <= 8391, u64 n <= 8391,
 *            u128 n <= 8391, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A335435 – T(n,k): minimal degree of a polynomial interpolating
   (i,prime(i)) for i = k … n.  The sequence a(n) is the triangle read
   by rows: a(1)=0, a(2)=1, a(3)=0, a(4)=2, …  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n=0, return 0 as safe default */

    /* Find row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((seq_t)r * (r + 1) / 2 < n) r++;

    /* Column k within that row (1‑based) */
    idx_t prev_tri = r * (r - 1) / 2;
    idx_t k = n - prev_tri;            /* k = column position */
    idx_t len = r - k + 1;             /* number of points to interpolate */

    /* Gather the prime values for indices k … r */
    uint64_t *pr = (uint64_t *)malloc(len * sizeof(uint64_t));
    if (!pr) return 0; /* memory failure */

    for (idx_t i = 0; i < len; ++i) {
        pr[i] = (uint64_t)get_prime(k + i);
    }

    /* Finite‑difference test for constant differences */
    uint64_t *cur = pr;
    idx_t sz = len;
    seq_t degree = 0;

    while (sz > 1) {
        idx_t next_len = sz - 1;
        uint64_t *next = (uint64_t *)malloc(next_len * sizeof(uint64_t));
        if (!next) { free(cur); return degree; }

        uint64_t first = cur[1] - cur[0];
        next[0] = first;
        bool const_flag = true;

        for (idx_t i = 1; i < next_len; ++i) {
            uint64_t diff = cur[i + 1] - cur[i];
            next[i] = diff;
            if (diff != first) const_flag = false;
        }

        free(cur);
        cur = next;
        sz = next_len;
        degree++;

        if (const_flag) break;
    }

    free(cur);
    return (seq_t)degree;
}
