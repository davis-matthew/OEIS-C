/*
 * A341309  Sum of odd divisors of n that are <= A003056(n).
 * https://oeis.org/A341309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d | n, d odd, d <= k} d, where k = A003056(n) is the
 *            largest k with k*(k+1)/2 <= n
 * Range:     u8 n <= 2924, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 107 terms through n = 9781 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0) return sum;
    for (idx_t d = 1; ; d += 2) {
        unsigned __int128 t = (unsigned __int128)d * ((unsigned __int128)d + 1) / 2;
        /* d <= k  <=>  d*(d+1)/2 <= n */
        if (t > (unsigned __int128)n) break;
        if (n % d == 0) sum += (seq_t)d;
    }
    return sum;
}
