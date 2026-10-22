/*
 * A384244  Triangle in which the n-th row gives the numbers k from 1 to n
 *          such that the greatest common unitary divisor of k and n is 1.
 * https://oeis.org/A384244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 8992, u16 n <= 8992, u32 n <= 8992, u64 n <= 8992,
 *            u128 n <= 8992, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th entry of the triangle whose r‑th row
   lists all integers k (1 ≤ k ≤ r) for which the greatest common unitary
   divisor of k and r equals 1.  Computation is performed by brute force
   enumeration of the rows up to the required position.  */

/* ------------------------------------------------------------------- */
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------- */
/* Factorise a number n into primes and their exponents.
   primes   – array to receive prime factors
   exps     – array to receive corresponding exponents
   cnt      – pointer to integer to receive count of distinct primes
   The arrays must be large enough to hold all distinct prime factors.   */
static void factor(idx_t n, idx_t primes[], int exps[], int *cnt)
{
    int c = 0;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            int e = 0;
            do
            {
                tmp   /= p;
                e     += 1;
            } while (tmp % p == 0);
            primes[c] = p;
            exps[c]   = e;
            ++c;
        }
    }
    if (tmp > 1)          /* remaining prime factor */
    {
        primes[c] = tmp;
        exps[c]   = 1;
        ++c;
    }
    *cnt = c;
}

/* ------------------------------------------------------------------- */
/* Return the number of admissible k for a given row r.  */
static idx_t admissible_count(idx_t r)
{
    if (r == 1)           /* r = 1 has only k = 1 */
        return 1;

    /* factor r */
    idx_t primes[32];
    int   exps[32];
    int   pcnt;
    factor(r, primes, exps, &pcnt);

    idx_t count = 0;
    for (idx_t k = 1; k <= r; ++k)
    {
        bool bad = false;
        for (int i = 0; i < pcnt && !bad; ++i)
        {
            idx_t p = primes[i];
            int   e = exps[i];
            int   vp = 0;
            idx_t t = k;
            while (t % p == 0)
            {
                ++vp;
                t /= p;
                if (vp > e)   /* we only care if vp == e */
                    break;
            }
            if (vp == e)
                bad = true;
        }
        if (!bad)
            ++count;
    }
    return count;
}

/* ------------------------------------------------------------------- */
/* Return the offset‑th admissible k in row r (1‑based).  */
static idx_t admissible_k(idx_t r, idx_t offset)
{
    if (r == 1)           /* only k = 1 */
        return 1;

    /* factor r */
    idx_t primes[32];
    int   exps[32];
    int   pcnt;
    factor(r, primes, exps, &pcnt);

    idx_t idx = 0;         /* number of admissible k seen so far */
    for (idx_t k = 1; k <= r; ++k)
    {
        bool bad = false;
        for (int i = 0; i < pcnt && !bad; ++i)
        {
            idx_t p = primes[i];
            int   e = exps[i];
            int   vp = 0;
            idx_t t = k;
            while (t % p == 0)
            {
                ++vp;
                t /= p;
                if (vp > e)
                    break;
            }
            if (vp == e)
                bad = true;
        }
        if (!bad)
        {
            ++idx;
            if (idx == offset)
                return k;
        }
    }
    /* Should never reach here if offset is valid */
    return 0;
}

/* ------------------------------------------------------------------- */
/* External function required by the problem statement.               */
seq_t a(idx_t n)
{
    if (n == 0)                 /* sequence is 1‑based, return 0 for safety */
        return 0;

    idx_t cumulative = 0;
    idx_t r = 0;
    while (cumulative < n)
    {
        ++r;
        idx_t len = admissible_count(r);
        cumulative += len;
    }

    /* Now cumulative >= n, r is the row containing the n-th term. */
    idx_t offset = n - (cumulative - admissible_count(r));
    idx_t value  = admissible_k(r, offset);
    return (seq_t)value;
}
