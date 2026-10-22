/*
 * A330020  Expansion of e.g.f. Sum_{k>=1} x^k / (k! * (1 - x^k)^k).
 * https://oeis.org/A330020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{d|n} (d + n/d - 2)! / (d! * (d - 1)! * (n/d -
 *            1)!)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fact = NULL;   /* fact[i] = i!  (big integer) */
static size_t fact_cap = 0;  /* current allocated size */

/* Ensure factorials are computed up to at least n. */
static void ensure_factorial(idx_t n)
{
    size_t need = (size_t)n + 1; /* we need indices 0..n inclusive */
    if (fact_cap < need)
    {
        /* Allocate or grow the array */
        seq_t *tmp = (seq_t *)realloc(fact, need * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        fact = tmp;

        /* Initialize new entries */
        if (fact_cap == 0)
        {
            fact[0] = (seq_t)1;
            fact_cap = 1;
        }
        for (size_t i = fact_cap; i < need; ++i)
        {
            fact[i] = fact[i-1] * (seq_t)i;
        }
        fact_cap = need;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    ensure_factorial(n);

    seq_t nf = fact[(size_t)n];
    seq_t sum = 0;

    /* Simple integer sqrt, n <= 10000 in test range */
    idx_t sq = 0;
    while ((sq + 1) * (sq + 1) <= n) ++sq;

    for (idx_t d = 1; d <= sq; ++d)
    {
        if (n % d == 0)
        {
            /* First divisor d */
            idx_t a_div = d;
            idx_t b_div = n / d;

            seq_t prod = nf / fact[(size_t)a_div];

            seq_t choose = fact[(size_t)(a_div + b_div - 2)];
            choose /= fact[(size_t)(a_div - 1)];
            choose /= fact[(size_t)(b_div - 1)];

            seq_t term = prod * choose;
            sum += term;

            /* Complementary divisor (b_div, a_div) if distinct */
            if (a_div != b_div)
            {
                idx_t a2 = b_div;
                idx_t b2 = a_div;

                prod = nf / fact[(size_t)a2];

                choose = fact[(size_t)(a2 + b2 - 2)];
                choose /= fact[(size_t)(a2 - 1)];
                choose /= fact[(size_t)(b2 - 1)];

                term = prod * choose;
                sum += term;
            }
        }
    }

    return sum;
}
