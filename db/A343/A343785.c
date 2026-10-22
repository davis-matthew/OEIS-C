/*
 * A343785  a(n) is completely multiplicative with a(p^e) = (-1)^e if p == 2
 *          (mod 3) and a(p^e) = 1 otherwise.
 * https://oeis.org/A343785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(3*n) = a(n) a(3*n+1) = 1 a(3*n+2) = -1 (Recursively reduces n
 *            to the lowest non‑zero ternary digit)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Keep dividing by 3 until n reaches 1, 2 or 5, 8, ... (i.e. not divisible by 3) */
    while (n % 3 == 0) {
        n /= 3;
    }
    /* After the loop n mod 3 is 1 or 2. */
    return (n % 3 == 1) ? (seq_t)1 : (seq_t)-1;
}
