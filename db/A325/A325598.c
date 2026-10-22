/*
 * A325598  Complement of A325597.
 * https://oeis.org/A325598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A325598 – Complement of A325597
 * Definition:
 *   c(0) = 1
 *   for k ≥ 1,  c(k) is the smallest integer > c(k‑1) that is not in
 *   { a(1), a(2), … , a(k‑1) }.
 *   a(k) = 2·c(k) + c(k‑1).
 *
 * The implementation recomputes the sequence up to the requested index n.
 * It keeps a bit array of already produced a-values to test membership of
 * candidates for c(k).  The algorithm runs in O(n) time and uses O(A(n))
 * memory, which is well within the limits for n ≤ 10 000.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Offset is 1: a(1) corresponds to the first term. */
    if (n == 0)
        return 0;              /* undefined for n = 0, return 0 */

    /* Initial capacity estimate: a(n) ≈ 4·n, so n*5+100 is safe. */
    size_t seen_cap = n * 5 + 100;
    if (seen_cap < 1) seen_cap = 1;

    unsigned char *seen = (unsigned char *)calloc(seen_cap, 1);
    if (seen == NULL)
        return 0;              /* allocation failure → undefined result */

    seq_t c_prev = 1;           /* c(0) */
    seq_t a_val  = 0;

    for (idx_t m = 1; m <= n; ++m)
    {
        /* Find the next c(m). */
        seq_t cand = c_prev + 1;
        for (;; ++cand)
        {
            size_t idx = (size_t)cand;
            if (idx >= seen_cap)
            {
                size_t new_cap = seen_cap * 2;
                while (idx >= new_cap)
                    new_cap *= 2;
                unsigned char *tmp = (unsigned char *)realloc(seen, new_cap);
                if (tmp == NULL)
                {
                    free(seen);
                    return 0;  /* allocation failure */
                }
                memset(tmp + seen_cap, 0, new_cap - seen_cap);
                seen = tmp;
                seen_cap = new_cap;
            }
            if (!seen[idx])
                break;           /* cand is not a previous a‑value */
        }
        seq_t c_cur = cand;

        /* Compute a(m) according to the formula. */
        a_val = 2 * c_cur + c_prev;

        /* Mark a(m) as seen. */
        size_t a_idx = (size_t)a_val;
        if (a_idx >= seen_cap)
        {
            size_t new_cap = seen_cap * 2;
            while (a_idx >= new_cap)
                new_cap *= 2;
            unsigned char *tmp = (unsigned char *)realloc(seen, new_cap);
            if (tmp == NULL)
            {
                free(seen);
                return 0;  /* allocation failure */
            }
            memset(tmp + seen_cap, 0, new_cap - seen_cap);
            seen = tmp;
            seen_cap = new_cap;
        }
        seen[a_idx] = 1;

        /* Prepare for the next iteration. */
        c_prev = c_cur;
    }

    free(seen);
    return a_val;
}
