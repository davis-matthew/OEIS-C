/*
 * A347049  Number of odd-length ordered factorizations of n with integer
 *          alternating product.
 * https://oeis.org/A347049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A347463(n) - A347048(n)
 * Range:     u8 n <= 575, u16 n <= 9243, u32 n <= 9243, u64 n <= 9243,
 *            u128 n <= 9243, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts odd‑length ordered factorizations of n with integer alternating product */
#include <stdlib.h>
#include <stdbool.h>

static void dfs(idx_t rem, idx_t parity, idx_t even_prod, idx_t n_orig, seq_t *sum)
{
    if (rem == 1) {
        if (parity & 1) {
            idx_t sq = even_prod * even_prod;
            if (n_orig % sq == 0)
                (*sum) += (seq_t)1;
        }
        return;
    }
    for (idx_t d = 2; d * d <= rem; ++d) {
        if (rem % d) continue;
        /* divisor d */
        idx_t new_even = even_prod;
        if (parity & 1)
            new_even *= d;
        if (new_even * new_even <= n_orig)
            dfs(rem / d, parity + 1, new_even, n_orig, sum);
        /* complementary divisor rem/d */
        idx_t e = rem / d;
        if (e != d) {
            idx_t new_even2 = even_prod;
            if (parity & 1)
                new_even2 *= e;
            if (new_even2 * new_even2 <= n_orig)
                dfs(rem / e, parity + 1, new_even2, n_orig, sum);
        }
    }
    /* factor rem itself */
    idx_t new_even3 = even_prod;
    if (parity & 1)
        new_even3 *= rem;
    if (new_even3 * new_even3 <= n_orig)
        dfs(1, parity + 1, new_even3, n_orig, sum);
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    seq_t sum = 0;
    dfs(n, 0, 1, n, &sum);
    return sum;
}
