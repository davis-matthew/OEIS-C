/*
 * A381311  Numbers whose powerful part (A057521) is a power of a prime with
 *          an even exponent >= 2.
 * https://oeis.org/A381311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A381311: numbers whose powerful part is a prime power with even exponent ≥ 2
   (i.e. numbers of the form p^(2m) × ∏ q_j, all q_j distinct primes) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Return true iff x satisfies the definition of A381311. */
static bool is_special(idx_t x)
{
    if (x < 4)              /* the smallest member is 4 */
        return false;

    idx_t n = x;
    int non_unit = 0;        /* count of primes with exponent > 1 */
    int even_exp = 0;        /* exponent of that prime (if any) */

    for (idx_t d = 2; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        int exp = 0;
        while (n % d == 0) {
            ++exp;
            n /= d;
        }

        if (exp == 1)        /* unitary prime factor */
            continue;

        ++non_unit;
        if (non_unit > 1)    /* more than one non‑unitary prime */
            return false;

        if (exp & 1)         /* exponent not even */
            return false;

        even_exp = exp;
    }

    /* any remaining n > 1 is a prime factor with exponent 1, already unitary */
    return (non_unit == 1) && (even_exp >= 2);
}

/* Return the n-th term of A381311 (n ≥ 1). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 4; ; ++k) {
        if (is_special(k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
