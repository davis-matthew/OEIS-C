/*
 * A295271  Number of n X 4 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0, 1 or 4 1s.
 * https://oeis.org/A295271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) +27*a(n-2) +63*a(n-3) +14*a(n-4) -110*a(n-5)
 *            -43*a(n-6) -45*a(n-7) +89*a(n-8) -150*a(n-9) +89*a(n-10)
 *            -126*a(n-11) +72*a(n-12) -30*a(n-13) +8*a(n-14) -2*a(n-15)
 *            +a(n-16); Initial values: a(1)=13, a(2)=98, a(3)=811, a(4)=6659,
 *            a(5)=54773, a(6)=449827, a(7)=3697742, a(8)=30386185, a...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   mul_coef
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_coef.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* Undefined for n = 0, not used */

    static seq_t *memo = NULL;
    static size_t memo_len = 0;
    static size_t memo_cap = 0;

    /* 16 initial terms (a(1) ... a(16)) */
    static const seq_t init[16] = {
        13ULL,
        98ULL,
        811ULL,
        6659ULL,
        54773ULL,
        449827ULL,
        3697742ULL,
        30386185ULL,
        249719021ULL,
        2052217315ULL,
        16865303569ULL,
        138600705864ULL,
        1139033988179ULL,
        9360690397437ULL,
        76927054505407ULL,
        632193927094967ULL
    };

    /* Coefficients for the recurrence */
    static const int coeff[16] = {
        4,   27,   63,   14,
       -110, -43, -45,   89,
       -150,  89, -126,  72,
       -30,    8,   -2,   1
    };

    /* Lazy allocation of memo table */
    if (memo == NULL) {
        memo_cap = 32;
        memo = (seq_t *)malloc(sizeof(seq_t) * memo_cap);
        for (int i = 0; i < 16; ++i)
            memo[i] = init[i];
        memo_len = 16;
    }

    /* Grow memo table if needed */
    while (memo_len < (size_t)n) {
        /* Ensure capacity */
        if (memo_len == memo_cap) {
            memo_cap <<= 1;
            memo = (seq_t *)realloc(memo, sizeof(seq_t) * memo_cap);
        }

        seq_t pos = 0;
        seq_t neg = 0;
        for (int i = 0; i < 16; ++i) {
            seq_t term = memo[memo_len - 1 - i];
            int c = coeff[i];
            if (c >= 0)
                pos += mul_coef(term, c);
            else
                neg += mul_coef(term, -c);
        }
        memo[memo_len] = pos - neg;
        memo_len++;
    }

    return memo[(size_t)n - 1];
}
