/*
 * A343044  Array T(n, k), n, k >= 0, read by antidiagonals; lunar addition
 *          table for the primorial base.
 * https://oeis.org/A343044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = lunar addition of the antidiagonal pair (r,s) in
 *            primorial base * where (r,s) are defined by * * m = floor(
 *            (sqrt(8*n+1)-1) / 2 ), * offset = n - m*(m+1)/2, * r = m -
 *            offset, * s = offset. * The lunar addition takes the digitwise
 *            maximum in primorial base (bases 2,3,5,7,…) * and returns th...
 * Range:     u8 n <= 10010, u16 n <= 10010, u32 n <= 10010, u64 n <= 10010,
 *            u128 n <= 10010, big any n
 * Validated: standard, pass, 107 terms through n = 9326 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Helper: primorial–base lunar addition of two non‑negative integers. */
static seq_t primorial_lunar(idx_t n, idx_t k)
{
    /* First 20 primes – more than enough for the expected ranges. */
    static const idx_t primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71
    };
    const size_t PRIMES_CNT = sizeof(primes)/sizeof(primes[0]);

    seq_t result   = 0;
    seq_t mult     = 1;
    idx_t a = n;
    idx_t b = k;

    for (size_t i = 0; i < PRIMES_CNT; ++i)
    {
        if (a == 0 && b == 0)
            break;

        idx_t p  = primes[i];
        idx_t da = a % p;  a /= p;
        idx_t db = b % p;  b /= p;
        idx_t d  = da > db ? da : db;

        result += (seq_t)d * mult;
        mult   *= (seq_t)p;
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find m such that m*(m+1)/2 <= n < (m+1)*(m+2)/2. */
    idx_t m = 0;
    while ((unsigned __int128)(m + 1) * (m + 2) / 2 <= n)
        ++m;

    /* Offset on the antidiagonal. */
    idx_t offset = n - (unsigned __int128)m * (m + 1) / 2;

    /* Convert to antidiagonal coordinates (r,s). */
    idx_t r = m - offset;
    idx_t s = offset;

    return primorial_lunar(r, s);
}
