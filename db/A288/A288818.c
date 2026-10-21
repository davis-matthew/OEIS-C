/*
 * A288818  Number of ways in which one can insert * and ^ into the decimal
 *          digits of n to create a valid (see comments) base-ten
 *          factorization statement.
 * https://oeis.org/A288818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of ways to insert * and ^ into the decimal digits
 *            of n so that the resulting phrase is a valid base‑ten
 *            factorization statement (product of powers of strictly
 *            increasing base‑ten primes)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"

static seq_t count_from(const char *dig, size_t L, size_t pos,
                       uint64_t last) {
    if (pos == L)          /* reached end – valid statement */
        return 1;

    if (dig[pos] == '0')   /* prime cannot start with zero */
        return 0;

    seq_t total = 0;
    uint64_t prime_val = 0;

    for (size_t prime_end = pos + 1; prime_end <= L; ++prime_end) {
        prime_val = prime_val * 10 + (dig[prime_end - 1] - '0');

        if (!is_prime(prime_val))
            continue;
        if (prime_val <= last)    /* must be strictly increasing */
            continue;

        /* Option 1: this group is the last one, no * or ^ after it */
        if (prime_end == L)
            total += 1;

        /* Option 2: use ^ and an exponent (if possible) */
        if (prime_end < L && dig[prime_end] != '0') {
            uint64_t exp_val = 0;
            for (size_t exp_end = prime_end + 1; exp_end <= L; ++exp_end) {
                exp_val = exp_val * 10 + (dig[exp_end - 1] - '0');
                if (exp_val < 2)      /* exponent 1 or 0 not allowed */
                    continue;
                total += count_from(dig, L, exp_end, prime_val);
            }
        }

        /* Option 3: insert * between primes (next group starts here) */
        if (prime_end < L) {
            total += count_from(dig, L, prime_end, prime_val);
        }
    }
    return total;
}

seq_t a(idx_t n) {
    if (n < 1)
        return 0;

    /* Convert n to its decimal digits (most significant first). */
    char digits[32];
    size_t L = 0;
    uint64_t tmp = (uint64_t)n;

    /* Handle n == 0 just in case */
    if (tmp == 0) {
        digits[0] = '0';
        L = 1;
    } else {
        while (tmp > 0) {
            digits[L++] = '0' + (tmp % 10);
            tmp /= 10;
        }
        /* reverse to correct order */
        for (size_t i = 0; i < L / 2; ++i) {
            char c = digits[i];
            digits[i] = digits[L - 1 - i];
            digits[L - 1 - i] = c;
        }
    }

    return count_from(digits, L, 0, 0);
}
