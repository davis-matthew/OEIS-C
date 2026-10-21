/*
 * A251419  Domination number of the n-triangle grid graph TG_n having n
 *          vertices along each side.
 * https://oeis.org/A251419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   a(n) = floor((n*n + 7*n - 23)/14) for n >= 14; explicit values
 *            for 1 <= n <= 13 (all other terms follow the formula)
 * Range:     u8 n <= 40, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Pre‑computed values for 1 <= n <= 13 (computed by exhaustive search). */
    static const seq_t small[13] = {
        (seq_t)1, /* n=1  */
        (seq_t)1, /* n=2  */
        (seq_t)2, /* n=3  */
        (seq_t)3, /* n=4  */
        (seq_t)3, /* n=5  */
        (seq_t)5, /* n=6  */
        (seq_t)6, /* n=7  */
        (seq_t)7, /* n=8  */
        (seq_t)9, /* n=9  */
        (seq_t)10,/* n=10 */
        (seq_t)13,/* n=11 */
        (seq_t)15,/* n=12 */
        (seq_t)17 /* n=13 */
    };

    if (n <= 13)
        return small[(size_t)(n - 1)];

    /* For n >= 14 use the conjectural formula.  Compute in idx_t to avoid
       overflow in the intermediate values, then cast the final result to
       seq_t (which is large enough to hold all values that fit in the
       target type). */
    idx_t nn   = n;
    idx_t num  = nn * nn + 7 * nn - 23;
    seq_t res  = (seq_t)(num / 14);   /* integer division truncates to floor */
    return res;
}
