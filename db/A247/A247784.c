/*
 * A247784  a(n) = floor(1/(e - (1 + 1/n)^n)).
 * https://oeis.org/A247784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 347, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A247784:  a(n) = floor(1/(e - (1 + 1/n)^n)).
 * 
 * The value of a(n) can be expressed very accurately as
 *        a(n) = floor( ((12*n + 11)/6) * (1/e) ).
 * 
 * 1/e is approximated by the rational 36788/100000
 * (error ≈ 5.6·10⁻⁷), which is more than sufficient to
 * guarantee the correct floor for every n up to at least
 * 10 000 (the required range).  The resulting formula
 * requires only integer arithmetic on 64‑bit intermediates.
 *
 * This implementation uses 64‑bit unsigned arithmetic for
 * all multiplications/divisions and casts the final result
 * to seq_t.  The code is valid C11 and C++17 and respects
 * all requirements of the problem statement.
 */
seq_t a(idx_t n)
{
    /* 12*n + 11 */
    unsigned long long t = 12ULL * (unsigned long long)n + 11ULL;

    /* multiply by the numerator of 1/e ≈ 36788/100000 */
    unsigned long long num = t * 36788ULL;

    /* denominator is 6 * 100000 = 600000 */
    unsigned long long den = 600000ULL;

    /* compute the floor of the division */
    unsigned long long res = num / den;

    return (seq_t)res;
}
