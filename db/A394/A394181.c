/*
 * A394181  Simultaneously grow three sequences S, T and A starting with
 *          S(1)=T(1)=a(1)=2. Search for the smallest integer m in S so that
 *          one of its multiples k*m is larger than T(n) and closer to T(n)
 *          than (k-1)*m. If this is true, a(n+1) = m and T(n+1) = k*m.
 *          Otherwise, add the smallest positive integer, m...
 * https://oeis.org/A394181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   in_S
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A394181
   The sequence is defined by the following process.
   Let S, T and a be sequences with S(1)=T(1)=a(1)=2.
   For n ≥ 1:
      - Search the smallest m∈S such that the next multiple
        km of m (the smallest multiple larger than T(n))
        is closer to T(n) than the previous multiple (k‑1)m.
        Equivalently, current_T % m > m/2.
      - If such an m exists, set a(n+1)=m and T(n+1)=k*m.
      - Otherwise, find the smallest positive integer not already in S
        that satisfies the inequality, add it to S, and use it as m
        for the current step.  Then set a(n+1)=m and T(n+1)=k*m.
   This function computes a(n) for any n≥1, memoising previously
   computed terms and the sets S and T.  All arithmetic is performed
   using the generic types seq_t and idx_t provided by the build system.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/in_S.h"

/* helper: grow an array to at least `need` elements */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t need)
{
    if (need > *cap) {
        size_t newcap = (*cap) ? (*cap) * 2 : 16;
        if (newcap < need) newcap = need;
        seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = newcap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static seq_t *S = NULL;      /* set S – unsorted */
    static size_t s_cnt = 0;
    static seq_t *T = NULL;      /* sequence T, T[0] = T(1) */
    static size_t t_cnt = 0;
    static seq_t *A = NULL;      /* sequence A, A[0] = a(1) */
    static size_t a_cnt = 0;
    static size_t capS = 0, capT = 0, capA = 0;
    static idx_t computed_n = 0; /* number of terms already computed (>=1) */

    /* initialise the base case */
    if (computed_n == 0) {
        ensure_capacity(&S, &capS, 1);      S[0] = (seq_t)2; s_cnt = 1;
        ensure_capacity(&T, &capT, 1);      T[0] = (seq_t)2; t_cnt = 1;
        ensure_capacity(&A, &capA, 1);      A[0] = (seq_t)2; a_cnt = 1;
        computed_n = 1;
    }

    /* compute terms up to requested index n (1‑based) */
    while (computed_n < n) {
        seq_t current_T = T[computed_n - 1];
        seq_t chosen_m = 0;
        size_t chosen_index = 0;
        bool found = false;
        seq_t min_m = 0;

        /* find smallest m∈S satisfying the condition */
        for (size_t i = 0; i < s_cnt; ++i) {
            seq_t m = S[i];
            seq_t rem = current_T % m;
            if (rem > m / 2) {
                if (!found || m < min_m) {
                    min_m = m;
                    chosen_index = i;
                    found = true;
                }
            }
        }

        if (!found) {
            /* find the smallest positive integer not in S that satisfies */
            seq_t candidate = 2;        /* 1 never satisfies a(n>1) */
            for (;;) {
                if (!in_S(candidate, S, s_cnt)) {
                    seq_t rem = current_T % candidate;
                    if (rem > candidate / 2) {
                        chosen_m = candidate;
                        /* append this new element to S */
                        ensure_capacity(&S, &capS, s_cnt + 1);
                        S[s_cnt++] = candidate;
                        break;
                    }
                }
                ++candidate;
            }
        } else {
            chosen_m = S[chosen_index];
        }

        /* compute the next T value: k = floor(current_T / m) + 1 */
        seq_t k = current_T / chosen_m + 1;
        seq_t chosen_Tnext = k * chosen_m;

        /* store results */
        ensure_capacity(&A, &capA, a_cnt + 1);
        A[a_cnt++] = chosen_m;
        ensure_capacity(&T, &capT, t_cnt + 1);
        T[t_cnt++] = chosen_Tnext;
        ++computed_n;
    }

    /* return requested term */
    return A[n - 1];
}
