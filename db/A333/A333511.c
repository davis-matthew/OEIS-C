/*
 * A333511  Number of self-avoiding walks in the n X 3 grid graph which start
 *          at any of the n vertices on left side of the graph and terminate
 *          at any of the n vertices on the right side.
 * https://oeis.org/A333511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/****************************************************
 *  OEIS A333511: Number of self‑avoiding walks in an n×3 grid
 *  from any vertex on the left side to any vertex on the right side.
 *
 *  The closed form generating function is
 *      G(x) = x*(1 + x)*(1 + 4*x - 37*x^2 + 80*x^3 - 34*x^4 - 38*x^5
 *               + 11*x^6 + 8*x^7 + x^8) /
 *           ((1 - x)^3*(1 - x + x^2)*(1 - 2*x - x^2)^2*(1 - 3*x - x^2)).
 *
 *  From this we obtain a linear recurrence of order 11:
 *
 *      a(n) = -C1*a(n-1) - C2*a(n-2) - … - C11*a(n-11) + B(n),
 *
 *  where the denominator coefficients Ck (k ≥ 0) are
 *      C = { 1, -11, 48, -106, 123, -61, -19,  41, -12, -8, 3, 1 }
 *  and the numerator coefficients B(n) are non‑zero only for n ≤ 10:
 *      B = {0, 1, 5, -33, 43, 46, -72, -27, 19, 9, 1 }.
 *
 *  The first non‑zero term is a(1)=1 and the sequence is defined
 *  for all n ≥ 1.  The function implements the recurrence with a
 *  simple memoisation cache so that successive calls (in any order)
 *  run in O(max(n)) time.
 *
 *  Note: The computation uses only unsigned arithmetic; the sign
 *  of each term is handled by separating positive and negative
 *  contributions into two accumulators.
 *****************************************************/

#include <stdlib.h>

static const int C[12] =
{
    1,  /* C0 – always 1, the leading coefficient of the denominator */
    -11, 48, -106, 123, -61, -19,  41, -12, -8, 3, 1
};

static const int B[11] =
{
    0, 1, 5, -33, 43, 46, -72, -27, 19, 9, 1
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* a(0) is not used in the sequence */

    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static idx_t   computed = 0;

    /* enlarge cache if needed */
    if (n >= cap)
    {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cap   = new_cap;
    }

    cache[0] = (seq_t)0;   /* a(0) = 0, used in the recurrence */

    /* compute missing terms */
    for (idx_t i = computed + 1; i <= n; ++i)
    {
        seq_t pos = 0;   /* contributions that increase the result */
        seq_t neg = 0;   /* contributions that decrease the result */

        /* numerator term B(i) – separates sign */
        int bi = (i <= 10) ? B[i] : 0;
        if (bi >= 0) pos += (seq_t)bi;
        else         neg += (seq_t)(-bi);

        /* accumulate terms from the recurrence */
        idx_t maxk = (i < 11) ? i : 11;
        for (idx_t k = 1; k <= maxk; ++k)
        {
            int ck   = C[k];
            seq_t prev = cache[i - k];
            if (ck > 0)
                neg += (seq_t)ck * prev;     /* subtract positive coefficient */
            else
                pos += (seq_t)(-ck) * prev;  /* add from negative coefficient */
        }

        /* the value is never negative */
        cache[i] = pos - neg;
    }

    computed = n;
    return cache[n];
}
