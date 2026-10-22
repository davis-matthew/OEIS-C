/*
 * A391108  a(n) is the number of cubic polynomials with coefficients in {-n,
 *          ..., n}, positive leading coefficient, and having three integer
 *          roots.
 * https://oeis.org/A391108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A391108:  number of cubic polynomials with coefficients in
 *  {-n,…,n}, positive leading coefficient, and three integer roots.
 *
 *  For a leading coefficient a (1 ≤ a ≤ n) let m = ⌊n/a⌋.
 *  A triple of roots (r1,r2,r3) (unordered, r1 ≤ r2 ≤ r3) is admissible
 *  iff |r1+r2+r3| ≤ m, |r1r2+r1r3+r2r3| ≤ m, and |r1r2r3| ≤ m.
 *  a(n) = Σ_{a=1}^n  T(⌊n/a⌋)   where T(m) counts such triples.
 *  T(m) is memoised; its computation is O(m²) using bounds on r3 for
 *  each pair (r1,r2).  The total work up to n is about 2·√n·n²,
 *  well below the 2 s limit for n ≤ 10 000.
 */

#include <stdlib.h>
#include <stdint.h>

/*-------------------------------------*/
/* signed integer division towards
 * floor (i.e. towards –∞).              */
static inline int64_t floor_div(int64_t a, int64_t b)
{
    int64_t q = a / b;
    int64_t r = a % b;
    if ((r != 0) && ((r > 0) != (b > 0)))
        --q;
    return q;
}
/*-------------------------------------*/
/* signed integer division towards
 * ceil (i.e. towards +∞).              */
static inline int64_t ceil_div(int64_t a, int64_t b)
{
    int64_t q = a / b;
    int64_t r = a % b;
    if ((r != 0) && ((r > 0) == (b > 0)))
        ++q;
    return q;
}
/*-------------------------------------*/
static seq_t *T_cache = NULL;          /* cache for T(m) */
static unsigned char *T_done = NULL;   /* 1 if cached, 0 otherwise */
static idx_t T_cache_len = 0;          /* cached size */

/* count admissible triples for a given bound m */
static seq_t count_triples(idx_t m_idx)
{
    if (m_idx >= T_cache_len) {
        size_t new_len = (size_t)(m_idx + 1);
        seq_t *new_cache = (seq_t *)realloc(T_cache,
                                            new_len * sizeof(seq_t));
        unsigned char *new_done = (unsigned char *)realloc(T_done,
                                        new_len * sizeof(unsigned char));
        if (!new_cache || !new_done) exit(EXIT_FAILURE);
        T_cache = new_cache;
        T_done  = new_done;
        for (size_t i = T_cache_len; i < new_len; ++i) T_done[i] = 0;
        T_cache_len = new_len;
    }
    if (T_done[m_idx]) return T_cache[m_idx];

    if (m_idx == 0) {                     /* only (0,0,0) works */
        T_done[0] = 1;
        T_cache[0] = (seq_t)1;
        return T_cache[0];
    }

    int64_t M = (int64_t)m_idx;           /* bound for roots */
    seq_t total = 0;

    for (int64_t r1 = -M; r1 <= M; ++r1) {
        for (int64_t r2 = r1; r2 <= M; ++r2) {
            int64_t s12  = r1 + r2;        /* r1 + r2 */
            int64_t p12  = r1 * r2;        /* r1 * r2 */

            int64_t L = r2;                /* r3 ≥ r2  (unordered) */
            int64_t U = M;                 /* r3 ≤ M */

            /* sum constraint: |s12 + r3| ≤ M  */
            int64_t L_sum = -M - s12;
            int64_t U_sum =  M - s12;
            if (L < L_sum) L = L_sum;
            if (U > U_sum) U = U_sum;
            if (L > U) continue;

            /* product constraint: |p12 * r3| ≤ M */
            if (p12 != 0) {
                int64_t L_prod, U_prod;
                if (p12 > 0) {
                    L_prod = ceil_div(-M, p12);
                    U_prod = floor_div( M, p12);
                } else { /* p12 < 0 */
                    L_prod = ceil_div( M, p12);
                    U_prod = floor_div(-M, p12);
                }
                if (L < L_prod) L = L_prod;
                if (U > U_prod) U = U_prod;
                if (L > U) continue;
            }

            /* S2 constraint: |p12 + r3*s12| ≤ M */
            if (s12 == 0) {
                if (p12 >  M || p12 < -M) continue;   /* no r3 satisfies */
            } else {
                int64_t L_s2, U_s2;
                if (s12 > 0) {
                    L_s2 = ceil_div(-M - p12, s12);
                    U_s2 = floor_div( M - p12, s12);
                } else { /* s12 < 0 */
                    L_s2 = ceil_div( M - p12, s12);
                    U_s2 = floor_div(-M - p12, s12);
                }
                if (L < L_s2) L = L_s2;
                if (U > U_s2) U = U_s2;
                if (L > U) continue;
            }

            /* all constraints satisfied for any r3 in [L,U] */
            total += (seq_t)(U - L + 1);
        }
    }

    T_done[m_idx] = 1;
    T_cache[m_idx] = total;
    return total;
}

/*-------------------------------------*/
/* public sequence function              */
seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t a_lead = 1; a_lead <= n; ++a_lead) {
        idx_t m = n / a_lead;          /* floor(n / a_lead) */
        res += count_triples(m);
    }
    return res;
}
