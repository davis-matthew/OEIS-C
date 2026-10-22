/*
 * A377656  Cogrowth sequence of the 12-element dicyclic group Dic12 = <S,T |
 *          S^6, T^4, STST^3, S^3T^2>.
 * https://oeis.org/A377656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in (2*x^6 + 3*x^5 + x^4 + 2*x^3 + x^2
 *            - 1) /* /((x + 1)*(2*x - 1)*(2*x^2 - x + 1)*(2*x^2 + 2*x + 1))
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) satisfies the linear recurrence: */
/*   a(n) = a(n-2) + 2*a(n-3) + 2*a(n-4) + 8*a(n-5) + 8*a(n-6)   for n ≥ 7 */
/* with initial values a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=5, a(6)=7. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;   /* next index to compute (size of valid data) */

seq_t a(idx_t n)
{
    /* Grow cache if necessary */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* abort on allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Pre‑computed coefficients for the numerator N(x) */
    const int rhs_term[7] = { -1, 0, 1, 2, 1, 3, 2 };

    /* Recurrence stall constants (Q(x) coefficients excluding q0=-1, q1=0) */
    /* q2=1, q3=2, q4=2, q5=8, q6=8 */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        seq_t sum = 0;
        if (i >= 2) sum += cache[i - 2];               /* q2 = 1 */
        if (i >= 3) sum += (seq_t)2 * cache[i - 3];    /* q3 = 2 */
        if (i >= 4) sum += (seq_t)2 * cache[i - 4];    /* q4 = 2 */
        if (i >= 5) sum += (seq_t)8 * cache[i - 5];    /* q5 = 8 */
        if (i >= 6) sum += (seq_t)8 * cache[i - 6];    /* q6 = 8 */

        if (i <= 6)
        {
            int rhs = rhs_term[i];
            if (rhs < 0)
                cache[i] = sum + (seq_t)(-rhs);  /* subtract negative rhs */
            else
                cache[i] = sum - (seq_t)rhs;      /* subtract positive rhs */
        }
        else
        {
            cache[i] = sum;  /* RHS = 0 for n > 6 */
        }
    }

    cache_size = n + 1;
    return cache[n];
}
