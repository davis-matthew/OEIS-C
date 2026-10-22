/*
 * A393958  a(n) is the number of ways to write n = p + r where p is a prime
 *          and r is an anagram of p.
 * https://oeis.org/A393958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = # of primes p < n such that n-p is an anagram of p
 *            (leading zeros not allowed)
 * Range:     u8 n <= 2965, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_frequencies, digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digits.h"
#include "../utils/digit_frequencies.h"

seq_t a(idx_t n)
{
    /* Rule from the OEIS entry: zero if n ≠ 6 and divisible by 3 */
    if (n % 3 == 0 && n != 6)
        return 0;

    if (n <= 2)
        return 0;

    /* Sieve of Eratosthenes for primes up to n-1 */
    bool *is_prime = (bool *)calloc((size_t)n + 1, sizeof(bool));
    if (!is_prime)
        return 0; /* out of memory – unlikely for n ≤ 10000 */
    for (idx_t i = 2; i < n; ++i)
        is_prime[i] = true;
    for (idx_t i = 2; i * i < n; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j < n; j += i)
                is_prime[j] = false;
        }
    }

    seq_t count = 0;
    unsigned int freq_p[10], freq_r[10];

    for (idx_t p = 2; p < n; ++p) {
        if (!is_prime[p])
            continue;
        idx_t r = n - p;
        /* leading zeros not allowed: r must have same number of digits as p */
        if (digits(p) != digits(r))
            continue;
        memset(freq_p, 0, sizeof(freq_p));
        memset(freq_r, 0, sizeof(freq_r));
        digit_frequencies((seq_t)p, freq_p);
        digit_frequencies((seq_t)r, freq_r);
        bool same = true;
        for (int d = 0; d < 10; ++d) {
            if (freq_p[d] != freq_r[d]) {
                same = false;
                break;
            }
        }
        if (same)
            ++count;
    }

    free(is_prime);
    return count;
}
