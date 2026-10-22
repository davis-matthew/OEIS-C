/*
 * A373255  Numbers k for which A003415(k) == +1 (mod 3), where A003415 is the
 *          arithmetic derivative.
 * https://oeis.org/A373255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th integer k (n ≥ 1) for which the
 * arithmetic derivative A003415(k) is congruent to +1 modulo 3.
 * It computes the term by scanning integers starting with 2 until
 * the n‑th qualifying number is found.  The algorithm factors each
 * candidate, evaluates d(k) modulo 3 using the prime‑factorisation
 * formula d(k) = Σ e_i·k/p_i, and checks the result.  The search is
 * repeated independently for each call, so the function works for
 * any order of n. */

#include <stdint.h>
#include <stdbool.h>

/* Return true if the arithmetic derivative of k is 1 (mod 3). */
static bool is_good(idx_t k)
{
    if (k < 2)
        return false;

    uint64_t n = (uint64_t)k;     /* k fits in uint64_t for n ≤ 10 000 */
    uint64_t d_mod3 = 0;
    uint64_t temp = n;

    /* trial division to factor n */
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            uint64_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            uint64_t n_div_p = n / p;
            d_mod3 = (d_mod3 + (e * (n_div_p % 3))) % 3;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1)
    {
        uint64_t p = temp;          /* p is prime */
        uint64_t e = 1;
        uint64_t n_div_p = n / p;
        d_mod3 = (d_mod3 + (e * (n_div_p % 3))) % 3;
    }

    return d_mod3 == 1;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined before the offset */

    idx_t found = 0;
    idx_t candidate = 2;

    while (found < n)
    {
        if (is_good(candidate))
            ++found;

        if (found == n)
            return (seq_t)candidate;

        ++candidate;
    }

    /* unreachable, but keep the compiler happy */
    return (seq_t)0;
}
