/*
 * A321348  a(n) = Sum_{d|n} tau(d^n), where tau() is the number of divisors
 *          (A000005).
 * https://oeis.org/A321348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 41, u32 n <= 329, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Product_{p^k || n} ((k + 1) * (n * k + 2) / 2)
 * (the sequence A321348)
 */
#include <stdint.h>
#include <stdbool.h>

/* Helper: factorize n and compute the product as described above. */
seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t temp = (uint64_t)n;
    if (temp == 1)
        return result;               /* a(1) = 1 */

    uint64_t i = 2;
    while (i * i <= temp) {
        if (temp % i == 0) {
            uint64_t k = 0;
            while (temp % i == 0) {
                temp /= i;
                k++;
            }
            seq_t term_num =
                (seq_t)(k + 1) * ((seq_t)n * (seq_t)k + (seq_t)2);
            seq_t term = term_num / (seq_t)2;
            result *= term;
        }
        i += (i == 2 ? 1 : 2);  /* skip even numbers after 2 */
    }
    if (temp > 1) {                     /* remaining prime factor */
        uint64_t k = 1;
        seq_t term_num =
            (seq_t)(k + 1) * ((seq_t)n * (seq_t)k + (seq_t)2);
        seq_t term = term_num / (seq_t)2;
        result *= term;
    }
    return result;
}
