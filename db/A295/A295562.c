/*
 * A295562  List of numbers whose middle Fibonomial coefficient (2n,n)_F is
 *          prime to 105.
 * https://oeis.org/A295562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = the n-th positive integer k such that
 *            gcd((2k,k)_F,105)=1. The algorithm tests each integer for the
 *            prime-period constraints (periods 4,5,8 for 3,5,7) and the
 *            modulo 8 condition
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Prime periods for 3,5,7 (A001177 values). */
static const idx_t periods[3] = { 4, 5, 8 };

/* Corresponding primes. */
static const idx_t primes[3] = { 3, 5, 7 };

/* Upper digit limits in each base (floor(p/2)). */
static const idx_t limits[3] = { 1, 2, 3 };

/* Return true if k satisfies the coprimality conditions. */
static bool is_good(idx_t k)
{
    /* k must be 0 or 1 modulo 8. */
    if ((k & 7ULL) != 0 && (k & 7ULL) != 1)
        return false;

    for (int i = 0; i < 3; ++i) {
        idx_t period = periods[i];
        idx_t prime  = primes[i];
        idx_t limit  = limits[i];

        idx_t rem = k % period;

        /* fractional part < 1/2  →  2*rem < period */
        if ((rem << 1) >= period)
            return false;

        /* integral part must have base‑prime digits ≤ limit */
        idx_t m = k / period;
        while (m) {
            idx_t digit = m % prime;
            if (digit > limit)
                return false;
            m /= prime;
        }
    }
    return true;
}

/* Return the n-th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n = 0; return 0 */
        return (seq_t)0;

    idx_t count = 0;
    idx_t k    = 1;

    for (;;) {
        if (is_good(k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
