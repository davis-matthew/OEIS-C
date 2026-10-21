/*
 * A278781  Numbers m such that A000041(m) is of the form 2^4 * k for odd k.
 * https://oeis.org/A278781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 600, u32 n <= 600, u64 n <= 600,
 *            u128 n <= 600, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A278781: numbers m such that the partition number p(m) has 2-adic
   valuation exactly 4, i.e. p(m) ≡ 16 (mod 32).  The function
   finds the n-th such m by iteratively computing p(k) modulo 32 for
   successive k with the pentagonal number recurrence.  */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* no work for indices below the offset */
    if (n < 1)
        return (seq_t)0;

    /* static memory to keep the state between calls */
    static idx_t   max_k = 0;       /* largest k for which p(k) is known   */
    static size_t  cap_p = 0;       /* capacity of p_mod32 array            */
    static seq_t * p_mod32 = NULL;  /* p(k) modulo 32, indexed by k        */

    static size_t  sol_len = 0;     /* number of found terms so far         */
    static size_t  sol_cap = 0;     /* capacity of sol array                */
    static seq_t * sol = NULL;      /* array of the found m values          */

    /* initialise storage on first call */
    if (p_mod32 == NULL) {
        cap_p = 256;
        p_mod32 = (seq_t *)malloc(cap_p * sizeof(seq_t));
        if (!p_mod32) return (seq_t)0;
        p_mod32[0] = (seq_t)1; /* p(0) = 1 */
        max_k = 0;
    }
    if (sol == NULL) {
        sol_cap = 256;
        sol = (seq_t *)malloc(sol_cap * sizeof(seq_t));
        if (!sol) return (seq_t)0;
        sol_len = 0;
    }

    /* generate terms until we have at least n of them */
    while (sol_len < n) {
        /* compute p(max_k+1) mod 32 */
        idx_t k = max_k + 1;
        if ((idx_t)(max_k + 1) >= cap_p) { /* grow array */
            size_t new_cap = cap_p * 2;
            if (new_cap < (size_t)(max_k + 1))
                new_cap = (size_t)(max_k + 1);
            seq_t *tmp = (seq_t *)realloc(p_mod32, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            p_mod32 = tmp;
            cap_p = new_cap;
        }

        int acc = 0; /* accumulate modulo 32 */
        for (idx_t j = 1; ; ++j) {
            idx_t pent1 = j * (3 * j - 1) / 2;
            if (pent1 > k) break;
            int sign = (j & 1) ? 1 : -1;         /* (-1)^(j+1) */
            acc += sign * (int)p_mod32[k - pent1];

            idx_t pent2 = j * (3 * j + 1) / 2;
            if (pent2 > k) continue;
            acc += sign * (int)p_mod32[k - pent2];
        }
        acc %= 32;
        if (acc < 0) acc += 32;
        p_mod32[k] = (seq_t)acc;

        /* if p(k) ≡ 16 (mod 32), we have a term with v₂(p(k)) = 4 */
        if (acc == 16) {
            if (sol_len + 1 > sol_cap) {        /* grow sol array */
                size_t new_cap = sol_cap * 2;
                if (new_cap < sol_len + 1)
                    new_cap = sol_len + 1;
                seq_t *tmp = (seq_t *)realloc(sol, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;
                sol = tmp;
                sol_cap = new_cap;
            }
            sol[sol_len++] = (seq_t)k;
        }

        max_k = k;
    }

    return sol[n - 1];
}
