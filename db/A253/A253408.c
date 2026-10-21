/*
 * A253408  Values of difference z-y that solve equation x^2 + y^2 = z^2 + 2.
 * https://oeis.org/A253408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = -1; for n > 1, a(n) is the (n‑1)th positive integer whose
 *            prime factors are all congruent to 1 or 7 modulo 8. (That is,
 *            a(n) = A058529(n‑1) for n > 1
 * Range:     i8 n <= 18, i16 n <= 56, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: test whether an integer has only prime factors ≡ ±1 (mod 8). */
static bool is_good(idx_t x)
{
    if (x == 1)                /* 1 has no prime factors, thus good */
        return true;
    if (x % 2 == 0)            /* even numbers (except 2) are bad */
        return false;

    idx_t temp = x;
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            if (p % 8 != 1 && p % 8 != 7)
                return false;
            while (temp % p == 0) temp /= p;
        }
    }
    /* remaining prime factor > sqrt(x) */
    if (temp > 1 && (temp % 8 != 1 && temp % 8 != 7))
        return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 1)                        /* the only negative entry */
        return (seq_t)-1;

    idx_t target = n - 1;              /* which positive entry to find */
    idx_t count  = 0;                  /* how many good numbers found */

    for (idx_t num = 1; ; ++num) {
        if (is_good(num)) {
            ++count;
            if (count == target)
                return (seq_t)num;
        }
    }
}
