/*
 * A373849  Difference between the primorial base exp-function and the
 *          arithmetic derivative.
 * https://oeis.org/A373849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 14, i16 n <= 57, i32 n <= 412, i64 n <= 2306,
 *            i128 n <= 9780, big any n
 * Helpers:   get_prime, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A276086(n) - A003415(n)
 * A276086: product of primes ^ (digits in primorial base of n)
 * A003415: arithmetic derivative of n
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Handle n = 0 separately */
    if (n == 0)
        return (seq_t)1;

    /* ---- Compute A276086(n) : primorial base exp-function ---- */
    seq_t product = 1;
    idx_t rem = n;
    idx_t prime_idx = 1;

    /* Store the primes and digits (maximum 64 digits suffices for n ≤ 10000) */
    struct { uint64_t p; int d; } digits[64];
    int pos = 0;

    while (rem > 0)
    {
        uint64_t p = get_prime(prime_idx);      /* i-th prime, 1‑based */
        int d = (int)(rem % p);                /* digit at this position */
        rem /= p;
        digits[pos].p = p;
        digits[pos].d = d;
        pos++;
        prime_idx++;
    }

    for (int i = 0; i < pos; ++i)
    {
        if (digits[i].d > 0)
            product *= pow_seq((seq_t)digits[i].p, digits[i].d);
    }

    /* ---- Compute A003415(n) : arithmetic derivative of n ---- */
    seq_t derivative = 0;
    idx_t temp = n;
    prime_idx = 1;

    while (temp > 1)
    {
        uint64_t p = get_prime(prime_idx);
        /* stop if p² > remaining factor */
        if ((idx_t)p * (idx_t)p > temp)
            break;

        int exp = 0;
        while (temp % p == 0)
        {
            temp /= p;
            exp++;
        }
        if (exp > 0)
            derivative += (seq_t)(n / p) * (seq_t)exp;

        prime_idx++;
    }

    /* Remaining prime factor, if any */
    if (temp > 1)
        derivative += (seq_t)(n / temp);

    /* ---- Return the difference ---- */
    return product - derivative;
}
