/*
 * A297200  Number of label-increasing forests with branching bounded by 4.
 * https://oeis.org/A297200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A297200
    Number of label‑increasing forests with branching bounded by 4
    Recurrence (derived from the differential equation
        F'(x) = \sum_{j=0}^{4} (F-1)^j/j! ,  F(0)=1 )
    Let a(n) be the sequence (a(0)=1, a(1)=1,...).
    Define c_{j,n} as the coefficient of x^n/n! in (F-1)^j.
    Then
        a(n+1) = \sum_{j=0}^{4}  c_{j,n} / j!          (j! = 1,1,2,6,24)
    The coefficients c_{j,n} satisfy
        c_{0,0}=1,   c_{j,0}=0  (j>0)
        c_{j,n} = \sum_{k=1}^{n} \binom{n}{k} a(k) c_{j-1,n-k}   (j≥1, n≥1)

    The following code evaluates a(n) for arbitrary n.
*/

#include <stddef.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#if defined(__cplusplus)
    /* C++ version – use std::vector for automatic construction */
    static std::vector<seq_t> a_vec;
    static std::vector<seq_t> c0_vec, c1_vec, c2_vec, c3_vec, c4_vec;
    static size_t max_computed = 0;
    if (max_computed == 0 && a_vec.empty())
    {
        a_vec.resize(1);
        c0_vec.resize(1);
        c1_vec.resize(1);
        c2_vec.resize(1);
        c3_vec.resize(1);
        c4_vec.resize(1);
        a_vec[0] = 1;
        c0_vec[0] = 1;
        c1_vec[0] = c2_vec[0] = c3_vec[0] = c4_vec[0] = 0;
        max_computed = 0;
    }

    /* enlarge capacity if necessary */
    size_t target = (size_t)n;
    if (target > max_computed)
    {
        a_vec.resize(target + 1);
        c0_vec.resize(target + 1);
        c1_vec.resize(target + 1);
        c2_vec.resize(target + 1);
        c3_vec.resize(target + 1);
        c4_vec.resize(target + 1);
        for (size_t i = max_computed + 1; i <= target; ++i)
        {
            c0_vec[i] = 0;
            c1_vec[i] = 0;
            c2_vec[i] = 0;
            c3_vec[i] = 0;
            c4_vec[i] = 0;
        }
    }

    /* constants */
    const seq_t fact[5] = {(seq_t)1, (seq_t)1, (seq_t)2, (seq_t)6, (seq_t)24};

    /* helper arrays */
    std::vector<seq_t> binom_vec;

    /* compute new terms up to n */
    for (size_t cur = max_computed; cur < target; ++cur)
    {
        /* compute a[cur+1] */
        seq_t sum = 0;
        sum += c0_vec[cur] / fact[0];
        sum += c1_vec[cur] / fact[1];
        sum += c2_vec[cur] / fact[2];
        sum += c3_vec[cur] / fact[3];
        sum += c4_vec[cur] / fact[4];
        a_vec[cur + 1] = sum;

        /* binomial coefficients for this n+1 */
        size_t m = cur + 1;
        binom_vec.resize(m + 1);
        binom_vec[0] = 1;
        for (size_t k = 1; k <= m; ++k)
            binom_vec[k] = binom_vec[k - 1] * (m - k + 1) / (seq_t)k;

        /* compute c1..c4 for index m */
        /* c_{1,m} = a[m]  (already holds after a_vec[cur+1] set) */
        for (int j = 2; j <= 4; ++j)
        {
            seq_t s = 0;
            for (size_t k = 1; k <= m; ++k)
            {
                const seq_t *prev =
                    (j == 2) ? &c1_vec[m - k] :
                    (j == 3) ? &c2_vec[m - k] :
                               &c3_vec[m - k];
                s += binom_vec[k] * a_vec[k] * (*prev);
            }
            if (j == 2) c2_vec[m] = s;
            else if (j == 3) c3_vec[m] = s;
            else c4_vec[m] = s;
        }
        /* c_{1,m} is already a_vec[m], but keep consistency with arrays */
        c1_vec[m] = a_vec[m];
    }

    max_computed = target;
    return a_vec[target];
#else
    /* C version – use plain malloc/free */
    static seq_t *a_vec = NULL;
    static seq_t *c0_vec = NULL;
    static seq_t *c1_vec = NULL;
    static seq_t *c2_vec = NULL;
    static seq_t *c3_vec = NULL;
    static seq_t *c4_vec = NULL;
    static size_t cap_a = 0, cap_c0 = 0, cap_c1 = 0, cap_c2 = 0, cap_c3 = 0, cap_c4 = 0;
    static size_t max_computed = 0;

    if (max_computed == 0 && a_vec == NULL)
    {
        cap_a = cap_c0 = cap_c1 = cap_c2 = cap_c3 = cap_c4 = 1;
        a_vec   = (seq_t*)malloc(cap_a   * sizeof(seq_t));
        c0_vec  = (seq_t*)malloc(cap_c0  * sizeof(seq_t));
        c1_vec  = (seq_t*)malloc(cap_c1  * sizeof(seq_t));
        c2_vec  = (seq_t*)malloc(cap_c2  * sizeof(seq_t));
        c3_vec  = (seq_t*)malloc(cap_c3  * sizeof(seq_t));
        c4_vec  = (seq_t*)malloc(cap_c4  * sizeof(seq_t));
        a_vec[0] = 1;
        c0_vec[0] = 1;
        c1_vec[0] = c2_vec[0] = c3_vec[0] = c4_vec[0] = 0;
        max_computed = 0;
    }

    size_t target = (size_t)n;
    if (target > max_computed)
    {
        size_t new_cap = target + 1;
        if (cap_a < new_cap) { a_vec   = (seq_t*)realloc(a_vec,   new_cap * sizeof(seq_t)); cap_a   = new_cap; }
        if (cap_c0 < new_cap) { c0_vec  = (seq_t*)realloc(c0_vec,  new_cap * sizeof(seq_t)); cap_c0  = new_cap; }
        if (cap_c1 < new_cap) { c1_vec  = (seq_t*)realloc(c1_vec,  new_cap * sizeof(seq_t)); cap_c1  = new_cap; }
        if (cap_c2 < new_cap) { c2_vec  = (seq_t*)realloc(c2_vec,  new_cap * sizeof(seq_t)); cap_c2  = new_cap; }
        if (cap_c3 < new_cap) { c3_vec  = (seq_t*)realloc(c3_vec,  new_cap * sizeof(seq_t)); cap_c3  = new_cap; }
        if (cap_c4 < new_cap) { c4_vec  = (seq_t*)realloc(c4_vec,  new_cap * sizeof(seq_t)); cap_c4  = new_cap; }
        for (size_t i = max_computed + 1; i <= target; ++i)
        {
            c0_vec[i] = 0;
            c1_vec[i] = 0;
            c2_vec[i] = 0;
            c3_vec[i] = 0;
            c4_vec[i] = 0;
        }
    }

    const seq_t fact[5] = {(seq_t)1, (seq_t)1, (seq_t)2, (seq_t)6, (seq_t)24};
    static seq_t *binom = NULL;
    static size_t cap_binom = 0;

    for (size_t cur = max_computed; cur < target; ++cur)
    {
        /* a[cur+1] */
        seq_t sum = 0;
        sum += c0_vec[cur] / fact[0];
        sum += c1_vec[cur] / fact[1];
        sum += c2_vec[cur] / fact[2];
        sum += c3_vec[cur] / fact[3];
        sum += c4_vec[cur] / fact[4];
        a_vec[cur + 1] = sum;

        size_t m = cur + 1;
        if (cap_binom < m + 1) { binom = (seq_t*)realloc(binom, (m + 1) * sizeof(seq_t)); cap_binom = m + 1; }
        binom[0] = 1;
        for (size_t k = 1; k <= m; ++k)
            binom[k] = binom[k - 1] * (m - k + 1) / (seq_t)k;

        for (int j = 2; j <= 4; ++j)
        {
            seq_t s = 0;
            seq_t *prev = (j == 2) ? c1_vec : (j == 3) ? c2_vec : c3_vec;
            for (size_t k = 1; k <= m; ++k)
                s += binom[k] * a_vec[k] * prev[m - k];
            if (j == 2) c2_vec[m] = s;
            else if (j == 3) c3_vec[m] = s;
            else c4_vec[m] = s;
        }
        /* c1 is a_vec itself */
        c1_vec[m] = a_vec[m];
    }

    max_computed = target;
    return a_vec[target];
#endif
}
