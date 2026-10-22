/*
 * A375311  a(1) = 0; for n >= 2, a(n) is the number of iterations needed for
 *          the map: x -> x / k if x is divisible by k, x -> x + 1 otherwise,
 *          to (the first occurrence of) 1. Divisor k starts at 2 and its
 *          value is increased by 1 after each division.
 * https://oeis.org/A375311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 762, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A375311: a(1)=0; for n≥2, a(n)= number of iterations
   required to reach 1 under the map
   x -> x/k if x divisible by k, else x+1,
   where k starts at 2 and increases by 1 after each division. */
seq_t a(idx_t n) {
    if (n == 1) return 0;
    seq_t x = (seq_t)n;       /* current value of x */
    idx_t k = 2;              /* current divisor */
    seq_t steps = 0;          /* iteration counter */

    while (x != 1) {
        if (x % (seq_t)k == 0) {
            x = x / (seq_t)k;
            ++k;
        } else {
            ++x;
        }
        ++steps;
    }
    return steps;
}
