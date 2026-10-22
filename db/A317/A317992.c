/*
 * A317992  2-rank of the narrow class group of the real quadratic field
 *          Q(sqrt(k)), k squarefree > 1.
 * https://oeis.org/A317992
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = omega(A005117(n)) - 1 if A005117(n) ≡ 1 or 2 (mod 4) a(n)
 *            = omega(A005117(n)) if A005117(n) ≡ 3 (mod 4) (A005117(n) is the
 *            n‑th square‑free integer; omega counts distinct prime factors.)
 * Range:     u8 n <= 434, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation computes the n‑th square‑free integer on demand
   and caches the results for repeated calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/bigomega.h"
#include "../utils/is_squarefree.h"

/* Return the n‑th term of sequence A317992 (n ≥ 2) */
seq_t a(idx_t n)
{
    /* Cache of square‑free numbers A005117.
       sqf[0] = 1, sqf[1] = 2, ... (0‑based to index easily) */
    static uint64_t *sqf      = NULL;   /* array of squarefree numbers */
    static size_t   sqf_cap   = 0;      /* allocated capacity */
    static size_t   sqf_len   = 0;      /* number of stored elements */
    static uint64_t next_cand = 1;      /* next integer to test for squarefree */

    /* Generate more squarefree numbers until we have at least n of them */
    while (sqf_len < n) {
        if (is_squarefree((seq_t)next_cand)) {
            if (sqf_len == sqf_cap) {    /* grow the array */
                size_t new_cap = sqf_cap ? sqf_cap * 2 : 128;
                uint64_t *tmp = (uint64_t *)realloc(sqf, new_cap * sizeof(uint64_t));
                if (!tmp) exit(1);       /* allocation failure */
                sqf      = tmp;
                sqf_cap  = new_cap;
            }
            sqf[sqf_len++] = next_cand;
        }
        ++next_cand;
    }

    uint64_t k     = sqf[n - 1];                 /* the n‑th squarefree number */
    seq_t    omega = bigomega((idx_t)k);         /* ω(k) */

    /* Apply the rule depending on k mod 4 */
    if ((k & 3) == 3)            /* k ≡ 3 (mod 4) */
        return omega;            /* ω(k) */
    else                         /* k ≡ 1 or 2 (mod 4) */
        return omega - 1;        /* ω(k) – 1 */
}
