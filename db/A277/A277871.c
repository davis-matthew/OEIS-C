/*
 * A277871  a(n) = Sum_{i=0..n+1} binomial(2*n-i,n-i+1)*Catalan(i).
 * https://oeis.org/A277871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n+1} binomial(2*n-i, n-i+1) * Catalan(i)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical test: a(0)=1, a(1)=4, a(2)=16, ... as in OEIS A277871 */

#include <stddef.h>
#include "../utils/binomial.h"   /* binomial(idx_t, idx_t) */
#include "../utils/catalan.h"    /* catalan(idx_t) */

seq_t a(idx_t n)
{
    /* Handle offset explicitly */
    if (n == 0)
        return (seq_t)1;

    /* Initial values */
    seq_t total = (seq_t)0;
    seq_t bi = binomial((idx_t)(2 * n), (idx_t)(n + 1)); /* B_0   */
    seq_t cat = catalan((idx_t)0);                       /* C_0   */

    /* Sum for i = 0 .. n-1 (terms B_i * C_i) */
    for (idx_t i = 0; i < n; ++i)
    {
        total += bi * cat;

        /* Update B_{i+1} = B_i * (n - i + 1) / (2*n - i) */
        seq_t num_b = (seq_t)(n - i + 1);
        seq_t den_b = (seq_t)(2 * n - i);
        bi = bi * num_b / den_b;

        /* Update C_{i+1} = C_i * 2*(2*i+1) / (i+2) */
        seq_t num_c = cat * (seq_t)2 * (seq_t)(2 * i + 1);
        seq_t den_c = (seq_t)(i + 2);
        cat = num_c / den_c;
    }

    /* Add the term for i = n  (B_n * C_n) */
    total += bi * cat;

    /* Compute C_{n+1} using the recurrence once more */
    seq_t num_c_next = cat * (seq_t)2 * (seq_t)(2 * n + 1);
    seq_t den_c_next = (seq_t)(n + 2);
    cat = num_c_next / den_c_next;

    /* Add the last term for i = n+1  (B_{n+1} = 1) */
    total += cat;   /* since B_{n+1} = 1 */

    return total;
}
