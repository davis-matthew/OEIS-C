/*
 * A382653  Numbers k such that a regular k-gon (k>=3) cannot be constructed
 *          with a compass, straightedge and an angle quinsector.
 * https://oeis.org/A382653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that a regular k-gon (k≥3) cannot be
 * constructed with a compass, straightedge and an angle quinsector.
 * The routine enumerates positive integers, skips the constructible
 * ones, and returns the n‑th non-constructible value (offset 1).  */
/* Implementation notes:
 *  - A k-gon is constructible iff
 *        k = 2^a * Π p_i
 *    where each odd prime factor p_i satisfies that p_i‑1 has only
 *    prime divisors 2 or 5, and each such prime occurs at most once,
 *    except 5 which may occur repeatedly (because quintisection
 *    allows constructing powers of 5).  Every other odd prime may
 *    appear only once.  2 may appear in any power.  Any deviation
 *    from this rule makes k non-constructible.  */

/* --- includes and type declarations --- */
/* The build supplies the types seq_t and idx_t (unsigned integer).  */
#include <stdbool.h>
#include <stdlib.h>

/* --- static state used for memoisation --- */
static seq_t *terms = NULL;           /* stored non-constructible k's */
static size_t terms_cnt = 0;          /* number of stored terms */
static size_t terms_cap = 0;          /* current capacity of `terms` */
static idx_t next_candidate = 3;      /* integer to test starting from 3 */

/* Utility: strip factors 2 and 5 from (p-1) and check if nothing remains. */
static bool good_prime(idx_t p)
{
    idx_t m = p - 1;
    while ((m & 1ULL) == 0U)          /* remove all powers of 2 */
        m >>= 1U;
    while (m % 5ULL == 0ULL)          /* remove all powers of 5 */
        m /= 5ULL;
    return m == 1ULL;                 /* only 2 or 5 were present */
}

/* Test if k is constructible under the above criterion. */
static bool is_constructible(idx_t k)
{
    /* remove all powers of 2 */
    while ((k & 1ULL) == 0ULL)
        k >>= 1ULL;
    if (k == 1ULL)                    /* k is just a power of 2          */
        return true;

    idx_t temp = k;
    for (idx_t d = 3ULL; d <= temp / d; d += 2ULL)   /* trial division */
    {
        if (temp % d == 0ULL)
        {
            int exp = 0;
            while (temp % d == 0ULL)
            {
                temp /= d;
                ++exp;
            }
            /* Repeated odd primes are forbidden except 5 */
            if (exp > 1 && d != 5ULL)
                return false;
            if (!good_prime(d))
                return false;
        }
    }
    if (temp > 1ULL)                   /* remaining prime factor (> sqrt)  */
    {
        /* exp = 1 for this prime, ok as long as it satisfies the rule */
        if (!good_prime(temp))
            return false;
    }
    return true;
}

/* Ensure the terms array has capacity for at least 'need' elements. */
static void grow_terms(size_t need)
{
    if (terms_cap < need)
    {
        size_t newcap = terms_cap ? terms_cap * 2 : 16;
        if (newcap < need)
            newcap = need;
        terms = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
        terms_cap = newcap;
    }
}

/* Return the n‑th term (1‑based) of the OEIS sequence.  */
seq_t a(idx_t n)
{
    if (n == 0)                           /* undefined, guard against 0    */
        return 0U;

    while (terms_cnt < n)
    {
        idx_t cand = next_candidate++;
        if (!is_constructible(cand))
        {
            grow_terms(terms_cnt + 1);
            terms[terms_cnt++] = (seq_t)cand;
        }
    }
    return terms[n - 1];
}
