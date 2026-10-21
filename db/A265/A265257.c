/*
 * A265257  Number of odd singletons in all partitions of n (n>=0).
 * https://oeis.org/A265257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=1..n} c_m · p(n‑m) where p(k) is the ordinary
 *            partition number, c_m = 1 if m≡1 or 3 (mod 4), -1 if m≡2 (mod
 *            4), 0 if m≡0 (mod 4)
 * Range:     u8 n <= 16, u16 n <= 41, u32 n <= 120, u64 n <= 394,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static state for cached values */
static seq_t *p_vec = NULL;           /* partition numbers */
static seq_t *a_vec = NULL;           /* a(n) values   */
static idx_t  cap   = 0;              /* current array capacity */
static idx_t  computed = 0;           /* number of values computed (next free index) */
static seq_t  p_mod[4] = {0, 0, 0, 0};/* sums of p[j] for j≡r (mod 4) */

/* extend the cache and fill missing entries up to index n  */
static void ensure_capacity(idx_t n)
{
    if (n < computed) return;  /* already computed */

    /* enlarge arrays if necessary */
    idx_t new_cap = cap;
    if (new_cap == 0) new_cap = 1;
    while (new_cap <= n) new_cap <<= 1;

    p_vec = (seq_t *)realloc(p_vec, new_cap * sizeof(seq_t));
    a_vec = (seq_t *)realloc(a_vec, new_cap * sizeof(seq_t));

    /* initialise new elements */
    for (idx_t i = cap; i < new_cap; ++i) {
        p_vec[i] = 0;
        a_vec[i] = 0;
    }
    cap = new_cap;

    /* initialisation when first called */
    if (computed == 0) {
        p_vec[0] = 1;          /* p(0) = 1 */
        a_vec[0] = 0;          /* a(0) = 0 */
        p_mod[0] = 1;          /* j = 0 contributes to mod 0 */
        computed = 1;
    }

    /* compute values up to n */
    for (idx_t i = computed; i <= n; ++i) {
        /* compute a[i] using the currently accumulated sums p_mod */
        seq_t val;
        switch (i & 3) {          /* i mod 4 */
            case 0: /* i ≡ 0 (mod 4) */
                val = p_mod[1] + p_mod[3];
                val -= p_mod[2];
                break;
            case 1: /* i ≡ 1 (mod 4) */
                val = p_mod[0] + p_mod[2];
                val -= p_mod[3];
                break;
            case 2: /* i ≡ 2 (mod 4) */
                val = p_mod[1] + p_mod[3];
                val -= p_mod[0];
                break;
            case 3: /* i ≡ 3 (mod 4) */
                val = p_mod[0] + p_mod[2];
                val -= p_mod[1];
                break;
        }
        a_vec[i] = val;

        /* compute p[i] using the pentagonal number recurrence */
        seq_t plus  = 0;
        seq_t minus = 0;
        for (int k = 1; ; ++k) {
            idx_t pent1 = (idx_t)k * ((idx_t)3 * k - 1) / 2;
            if (pent1 > i) break;
            seq_t term = p_vec[i - pent1];
            if (k & 1) plus += term; else minus += term;

            idx_t pent2 = (idx_t)k * ((idx_t)3 * k + 1) / 2;
            if (pent2 > i) continue;
            term = p_vec[i - pent2];
            if (k & 1) plus += term; else minus += term;
        }
        p_vec[i] = plus - minus;

        /* add p[i] to the appropriate modulo‑4 bucket for future terms */
        p_mod[i & 3] += p_vec[i];
    }

    computed = n + 1;
}

/* Public routine: a(n) for any non‑negative n */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safe guard for n = 0 */
    ensure_capacity(n);
    return a_vec[n];
}
