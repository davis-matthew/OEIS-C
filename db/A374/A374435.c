/*
 * A374435  Triangle read by rows: T(n, k) = Product_{p in PF(n) difference
 *          PF(k)} p, where PF(a) is the set of the prime factors of a.
 * https://oeis.org/A374435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) * where r = floor((sqrt(8*n+1)-1)/2) is the row
 *            index, * k = n - r*(r+1)/2 is the column index within that row,
 *            * and * T(r,k) = ∏_{p∈PF(r) \ PF(k)} p , * i.e. the product of
 *            all distinct prime factors of r that do NOT * divide k. It is
 *            implemented by computing rad(r)/rad(gcd(r,k))....
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    /* determine row r and column k */
    idx_t r = (idx_t)((sqrt((double)(8 * n + 1)) - 1) / 2);
    idx_t k = n - r * (r + 1) / 2;

    /* T(r, k) is the product of primes of r not dividing k   */
    seq_t prod = 1;
    if (r == 0) return prod;   /* first term is 1 */

    idx_t x = r;
    for (idx_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            /* p is a distinct prime factor of r */
            if (k == 0 || (k % p != 0))
                prod *= (seq_t)p;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) {            /* remaining prime factor */
        if (k == 0 || (k % x != 0))
            prod *= (seq_t)x;
    }
    return prod;
}
