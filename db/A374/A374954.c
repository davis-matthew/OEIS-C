/*
 * A374954  Positive integers k for which sqrt(k) < sqrt(p_1) + ... +
 *          sqrt(p_r), where p_1*...*p_r is the prime factorization of k.
 * https://oeis.org/A374954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the nth positive integer k such that sqrt(k) < sqrt(p1) +
 *            sqrt(p2) + ... + sqrt(pr), where p1* p2 * ... * pr is the prime
 *            factorisation of k
 * Range:     u8 n <= 19, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

seq_t a(idx_t n)
{
    /* Sequence A374954 (finite, ending at 64). */
    static const seq_t data[] = {
        4, 6, 8, 9, 10,
        12, 14, 15, 16, 18,
        20, 22, 24, 28, 32,
        36, 40, 48, 64
    };
    const size_t len = sizeof(data) / sizeof(data[0]);

    if (n == 0) return 0;              /* undefined for n < 1 */
    if (n <= len) return data[n - 1];
    return 0;                           /* no further terms */
}
