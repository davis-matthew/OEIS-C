/*
 * A382188  Expansion of 1/(1 - 9 * Sum_{k>=0} x^(2^k))^(1/3).
 * https://oeis.org/A382188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in 1/(1-9*Σ_{k≥0}x^{2^k})^{1/3}
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses the functional equation B(x)=A(x)^3 and a recurrence for
 * B_n = coefficient of x^n in B(x). Then a(n) is obtained from
 * B_n and the lower coefficients of a.  It is memoised for arbitrary
 * order of calls. */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for computed coefficients */
    static seq_t *a_cache = NULL;     /* a[0..m] */
    static size_t a_cap = 0;          /* current allocated size */
    static seq_t *b_cache = NULL;     /* B[n] = coeff of x^n in A^3, same as T */
    static size_t b_cap = 0;
    static const seq_t ONE = 1;
    static const seq_t TWO = 2;
    static const seq_t NINE = 9;
    static const seq_t THREE = 3;

    /* grow cache if necessary */
    if (n + 1 > a_cap) {
        size_t new_cap = (n + 1) * 2;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        b_cache = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
        /* initialise new elements */
        for (size_t i = a_cap; i < new_cap; ++i) {
            a_cache[i] = 0;
            b_cache[i] = 0;
        }
        a_cap = new_cap;
        b_cap = new_cap;
    }

    /* base case n==0 */
    if (n == 0) {
        /* initialise a[0] and b[0] if not already */
        if (a_cache[0] == 0) {
            a_cache[0] = ONE;
            b_cache[0] = ONE; /* A^3 at x^0 is 1^3 = 1 */
        }
        return a_cache[0];
    }

    /* fill cache up to n if needed */
    for (size_t cur = (n > 0) ? a_cache[0] == 0 ? 0 : 1ULL : 0; cur <= (size_t)n; ++cur) {
        if (cur == 0) continue; /* already initialised */

        /*--- compute B[cur] using recurrence B_n = 9 * Σ_{powers of two <= n} B[n-2^k] ---*/
        seq_t sum_b = 0;
        idx_t pow = 1;
        while (pow <= cur) {
            sum_b += b_cache[cur - pow];
            pow <<= 1;
        }
        b_cache[cur] = sum_b * NINE;

        /*--- compute sum_lower = Σ_{i+j+k=cur, all < cur} a[i]a[j]a[k] ---*/
        seq_t sum_lower = 0;

        /* i == 0 case */
        for (idx_t j = 1; j < cur; ++j) {          /* j cannot be 0 (k would equal cur) */
            idx_t k = cur - j;                     /* k < cur automatically */
            sum_lower += a_cache[0] * a_cache[j] * a_cache[k];
        }

        /* i >= 1 case */
        for (idx_t i = 1; i < cur; ++i) {
            for (idx_t j = 0; j <= cur - i; ++j) {   /* j can be 0 .. n-i */
                idx_t k = cur - i - j;               /* k < cur automatically */
                sum_lower += a_cache[i] * a_cache[j] * a_cache[k];
            }
        }

        /*--- solve for a[cur] ---*/
        a_cache[cur] = (b_cache[cur] - sum_lower) / THREE;
    }

    /* return requested coefficient */
    return a_cache[n];
}
