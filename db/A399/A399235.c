/*
 * A399235  G.f. A(x) satisfies: A(x) = 1 + x * A(x/(1 - 2*x)^2) / (1 - x)^2.
 * https://oeis.org/A399235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = Sum_{k=0..n-1} Sum_{j=0..n-k-1} binomial(2*k+j-1,j) *
 *            (n-k-j) * 2^j * a(k) (OEIS A399235)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *a_cache = NULL;
    static seq_t *S     = NULL;
    static seq_t *T     = NULL;
    static seq_t *binom_arr = NULL;
    static seq_t *pow2_arr  = NULL;
    static size_t *m_cur   = NULL;
    static size_t a_cap = 0, a_len = 0;

    /* enlarge working storage if necessary */
    if (n + 1 > a_cap) {
        size_t new_cap = n + 1;
        a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        S       = (seq_t*)realloc(S,       new_cap * sizeof(seq_t));
        T       = (seq_t*)realloc(T,       new_cap * sizeof(seq_t));
        binom_arr = (seq_t*)realloc(binom_arr, new_cap * sizeof(seq_t));
        pow2_arr  = (seq_t*)realloc(pow2_arr,  new_cap * sizeof(seq_t));
        m_cur     = (size_t*)realloc(m_cur,    new_cap * sizeof(size_t));

        /* initialise new entries */
        for (size_t i = a_cap; i < new_cap; ++i) {
            S[i]        = (seq_t)0;
            T[i]        = (seq_t)0;
            binom_arr[i] = (seq_t)1;
            pow2_arr[i]  = (seq_t)1;
            m_cur[i]     = (size_t)0;
        }
        a_cap = new_cap;
    }

    /* initial value a(0)=1 */
    if (a_len == 0) {
        a_cache[0] = (seq_t)1;
        a_len = 1;
    }

    /* compute values up to n */
    for (size_t i = a_len; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (size_t k = 0; k <= i-1; ++k) {
            size_t m_needed = i - k;            /* desired S_k(m_needed) */
            /* increment S_k and T_k until m_cur[k] == m_needed */
            while (m_cur[k] < m_needed) {
                /* term for current j = m_cur[k] */
                seq_t term = binom_arr[k] * pow2_arr[k];
                T[k] += term;
                S[k] += T[k];
                /* prepare next binomial */
                size_t j = m_cur[k];
                seq_t mult = (seq_t)(2 * k + j);
                binom_arr[k] = binom_arr[k] * mult / ( (seq_t)(j + 1) );
                pow2_arr[k] <<= 1;                 /* multiply by 2 */
                m_cur[k]++;                         /* m increases by 1 */
            }
            sum += S[k] * a_cache[k];
        }
        a_cache[i] = sum;
    }
    a_len = n + 1;
    return a_cache[n];
}
