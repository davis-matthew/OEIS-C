/*
 * A323076  Number of iterations of map x -> 1+(x-(largest divisor d < x)),
 *          starting from x=n, needed to reach a fixed point, which is always
 *          either a prime or 1.
 * https://oeis.org/A323076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of the map x -> 1 + (x - largest
 *            proper divisor of x) starting from n, until a fixed point (a
 *            prime or 1) is reached
 * Range:     u8 n <= 8954, u16 n <= 8954, u32 n <= 8954, u64 n <= 8954,
 *            u128 n <= 8954, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: smallest prime factor of n (returns n if n is prime or 1). */
static inline idx_t spf(idx_t n) {
    if (n < 2) return n;
    if ((n & 1) == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0) return i;
    }
    return n;
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;          /* 1 is a fixed point */
    idx_t p = spf(n);
    if (p == n) return 0;          /* n is prime → fixed point */
    idx_t next = 1 + n - n / p;    /* next value in the iteration */
    return 1 + a(next);
}
