/*
 * A350640  a(n) is the minimum lcm of the part sizes of a partition of n into
 *          parts of size 3 or more.
 * https://oeis.org/A350640
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4622, u16 n <= 4622, u32 n <= 4622, u64 n <= 4622,
 *            u128 n <= 4622, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the minimal divisor of n that is ≥3; if none ≤12, return 12. */

/* Function that computes A350640 for any n ≥ 3. */
seq_t a(idx_t n)
{
    /* Handle the special divisors in ascending order. */
    if (n % 3 == 0)  return (seq_t)3;
    if (n % 4 == 0)  return (seq_t)4;
    if (n % 5 == 0)  return (seq_t)5;
    if (n % 7 == 0)  return (seq_t)7;
    if (n % 11 == 0) return (seq_t)11;
    /* If none of the above divisors apply, the minimal lcm is 12. */
    return (seq_t)12;
}
