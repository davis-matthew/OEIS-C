/*
 * A258774  a(n) = 1 + sigma(n) + sigma(n)^2.
 * https://oeis.org/A258774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + sigma(n) + sigma(n)^2
 * Range:     u8 n <= 9, u16 n <= 107, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sigma(n) = sum of all positive divisors of n. */

#include <stdint.h>

/* Compute sigma(n) for a 64‑bit integer n using trial division.
   All arithmetic is performed in seq_t, which may be 8, 16, 32, 64 or 128
   bits, or an arbitrary‑precision type. */
static seq_t sigma_of_uint64(uint64_t n)
{
    seq_t result = (seq_t)1;       /* multiplicative identity for sigma */
    uint64_t temp = n;

    /* Handle factor 2 */
    if ((temp & 1U) == 0) {
        seq_t sum = (seq_t)1;
        seq_t power = (seq_t)1;
        while ((temp & 1U) == 0) {
            temp >>= 1;                 /* temp /= 2 */
            power *= (seq_t)2;
            sum += power;
        }
        result *= sum;
    }

    /* Handle factor 3 */
    if (temp % 3U == 0) {
        seq_t sum = (seq_t)1;
        seq_t power = (seq_t)1;
        while (temp % 3U == 0) {
            temp /= 3U;
            power *= (seq_t)3;
            sum += power;
        }
        result *= sum;
    }

    /* Handle remaining factors with 6k ± 1 wheel */
    for (uint64_t p = 5; p * p <= temp; p += 6) {
        if (temp % p == 0) {
            seq_t sum = (seq_t)1;
            seq_t power = (seq_t)1;
            while (temp % p == 0) {
                temp /= p;
                power *= (seq_t)p;
                sum += power;
            }
            result *= sum;
        }
        uint64_t p2 = p + 2;
        if (temp % p2 == 0) {
            seq_t sum = (seq_t)1;
            seq_t power = (seq_t)1;
            while (temp % p2 == 0) {
                temp /= p2;
                power *= (seq_t)p2;
                sum += power;
            }
            result *= sum;
        }
    }

    /* If a prime factor remains */
    if (temp > 1) {
        seq_t sum = (seq_t)1 + (seq_t)temp;
        result *= sum;
    }

    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;          /* cast index to standard integer */
    seq_t s = sigma_of_uint64(nn);      /* sigma(n) */
    seq_t s2 = s * s;                   /* sigma(n)^2 */
    return (seq_t)1 + s + s2;            /* 1 + sigma + sigma^2 */
}
