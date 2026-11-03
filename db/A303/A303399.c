/*
 * A303399  Number of ordered pairs (a, b) with 0 <= a <= b such that n - 5^a
 *          - 5^b can be written as the sum of two triangular numbers.
 * https://oeis.org/A303399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (a,b) with 0 ≤ a ≤ b such that *
 *            n – 5^a – 5^b can be expressed as the sum of two triangular
 *            numbers
 * Range:     u8 n <= 69, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9627 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool sum_of_two_triangular(seq_t m)
{
    /* Convert m into an integer of type seq_t and test
     * 4*m + 1 for representability as a sum of two squares. */
    seq_t val = m * 4 + 1;

    /* Factor val by trial division; a prime factor ≡ 3 (mod 4)
     * must occur with an even exponent. */
    seq_t x = val;               /* current factor to test */
    /* Remove factors of 2 – they are harmless */
    while ((x & 1) == 0)
        x >>= 1;

    /* Test odd prime factors */
    for (idx_t p = 3; (seq_t)p * p <= x; p += 2)
    {
        if (x % p == 0)
        {
            int exp_even = 0;    /* parity of exponent of p */
            while (x % p == 0)
            {
                x /= p;
                exp_even ^= 1;   /* flip parity */
            }
            if ((p % 4 == 3) && exp_even)  /* odd exponent ≡ 3 mod 4 */
                return false;
        }
    }

    /* Remaining x (if > 1) is a prime factor; check it */
    if (x > 1 && (x % 4 == 3))
        return false;

    return true;
}

seq_t a(idx_t n)
{
    /* Pre-compute powers of 5 once */
    static uint64_t pow5[65];
    static int pow5cnt;
    static bool initialized = false;
    if (!initialized)
    {
        pow5[0] = 1;
        pow5cnt = 1;
        while (pow5cnt < 65)
        {
            uint64_t next = pow5[pow5cnt - 1] * 5ULL;
            if (next / 5ULL != pow5[pow5cnt - 1])  /* overflow check */
                break;
            pow5[pow5cnt] = next;
            pow5cnt++;
        }
        initialized = true;
    }

    seq_t count = 0;

    /* Iterate over all pairs (a, b) with 0 ≤ a ≤ b */
    for (int a = 0; a < pow5cnt && pow5[a] <= n; a++)
    {
        for (int b = a; b < pow5cnt; b++)
        {
            seq_t sum = (seq_t)pow5[a] + (seq_t)pow5[b];
            if (sum > n)
                break;  /* further b will only increase the sum */
            seq_t m = (seq_t)n - sum;
            if (sum_of_two_triangular(m))
                count++;
        }
    }

    return count;
}
