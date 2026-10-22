/*
 * A307933  Number of (undirected) Hamiltonian paths in the n-antiprism graph.
 * https://oeis.org/A307933
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) - 4*a(n-3) + 7*a(n-4) + 5*a(n-5) -
 *            5*a(n-6) - 3*a(n-7) + a(n-8) + a(n-9) for n>11 Initial values
 *            (offset 3): a(3)=120, a(4)=408, a(5)=1200, a(6)=3240, a(7)=8330,
 *            a(8)=20720, a(9)=50418, a(10)=120760, a(11)=285846
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;      /* value undefined for n < 3 */

    /* static cache: memoization of computed terms */
    static seq_t *memo = NULL;
    static idx_t memo_start = 3;   /* the n value that memo[0] represents */
    static idx_t memo_end   = 11;  /* highest n computed so far */

    /* coefficients of the recurrence (a(n-1) ... a(n-9)) */
    static const int coeff[9] = {
        5,  -6,  -4,   7,   5,   -5,   -3,   1,   1
    };
    /* initial known values a(3) .. a(11) */
    static const seq_t base_vals[9] = {
        (seq_t)120,
        (seq_t)408,
        (seq_t)1200,
        (seq_t)3240,
        (seq_t)8330,
        (seq_t)20720,
        (seq_t)50418,
        (seq_t)120760,
        (seq_t)285846
    };

    /* initialise cache on first call */
    if (memo == NULL) {
        size_t sz = (size_t)(memo_end - memo_start + 1);
        memo = (seq_t *)malloc(sz * sizeof(seq_t));
        /* assume malloc succeed */
        for (size_t i = 0; i < sz; ++i) memo[i] = base_vals[i];
    }

    /* compute up to requested n if needed */
    if (n > memo_end) {
        size_t new_sz = (size_t)(n - memo_start + 1);
        if (new_sz > (size_t)(memo_end - memo_start + 1)) {
            seq_t *tmp = (seq_t *)realloc(memo, new_sz * sizeof(seq_t));
            if (!tmp) return 0;   /* allocation failed; caller will handle */
            memo = tmp;
        }
        for (idx_t m = memo_end + 1; m <= n; ++m) {
            seq_t pos = 0, neg = 0;
            for (int i = 0; i < 9; ++i) {
                idx_t k = m - 1 - i;             /* index of a(m-i)  */
                seq_t val = memo[k - memo_start];/* value stored in memo */
                if (coeff[i] >= 0)
                    pos += (seq_t)coeff[i] * val;
                else
                    neg += (seq_t)(-coeff[i]) * val;
            }
            memo[m - memo_start] = pos - neg;
        }
        memo_end = n;
    }

    return memo[n - memo_start];
}
