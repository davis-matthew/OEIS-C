/*
 * A351660  G.f. A(x) satisfies: A(x) = 1 + x + x^2 + x^3 * A(x/(1 - x)) / (1
 *          - x)^2.
 * https://oeis.org/A351660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 0,1,2; a(n) = Sum_{k=0}^{n-3} binomial(n-2,
 *            k+1) * a(k) for n >= 3
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  seq_t and idx_t are provided by the build system   */
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache: buf[0..len-1] holds computed values */
    static seq_t *buf   = NULL;
    static idx_t  buf_len = 0;
    static idx_t  old_len = 0;

    if (n < 3)                       /* a(0)=a(1)=a(2)=1 */
        return (seq_t)1;

    if (n >= buf_len)                /* need to grow cache */
    {
        old_len = buf_len;
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(buf, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* out of memory – 0 is a safe default */
        buf = tmp;
        buf_len = new_len;

        /* initialise base values */
        if (old_len == 0) {
            buf[0] = buf[1] = buf[2] = (seq_t)1;
        } else {                     /* base travelled over during resize */
            if (old_len <= 3) {
                buf[0] = buf[1] = buf[2] = (seq_t)1;
            }
        }

        /* compute new entries */
        for (idx_t i = (old_len > 3 ? old_len : 3); i <= n; ++i)
        {
            seq_t sum = 0;
            idx_t m   = i - 2;                   /* n-2 */
            seq_t comb = (seq_t)m;                /* C(m,1) */
            for (idx_t k = 0; k <= i - 3; ++k)
            {
                sum += comb * buf[(size_t)k];    /* add binomial * a(k) */
                if (k < i - 3) {
                    /* next binomial: C(m, k+2) = C(m, k+1) * (m-(k+1))/(k+2) */
                    comb = comb * (seq_t)(m - (k + 1)) / (seq_t)(k + 2);
                }
            }
            buf[(size_t)i] = sum;
        }
    }
    return buf[(size_t)n];
}
