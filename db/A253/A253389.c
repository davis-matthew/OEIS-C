/*
 * A253389  a(n) is the repeating digit pattern in penultimate digit of
 *          successive powers of n (omitting initial powers without at least
 *          two digits).
 * https://oeis.org/A253389
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 2, u128 n <= 30, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the repeating penultimate‑digit pattern for powers of n.
 * The function returns the concatenation of the period digits that
 * repeats indefinitely when we look at the tens digit of n^k
 * for k with n^k having at least two decimal digits.
 *
 * For n < 2 the function returns 0, as the sequence is only defined
 * for n ≥ 2.  This guard also protects against an infinite loop for
 * n = 0 or 1.
 *
 * The algorithm multiplies the remainder modulo 100 by n_mod_100 each
 * step, records the tens digit (remainder/10) until a remainder is
 * repeated.  The repeating cycle of remainders determines the
 * eventual cycle of tens digits; the digits of that cycle are then
 * concatenated into a seq_t value.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    /* Remainder of successive powers modulo 100. */
    const uint64_t base = (uint64_t)n % 100ULL;   /* n mod 100 */
    uint64_t r = base;
    uint64_t pow_val = (uint64_t)n;                /* actual power value */

    /* Skip initial powers that have only one decimal digit. */
    while (pow_val < 10ULL) {
        pow_val *= (uint64_t)n;
        r = (r * base) % 100ULL;
    }

    /* Track first occurrence of each remainder. */
    int rem_index[100];
    for (int i = 0; i < 100; ++i)
        rem_index[i] = -1;

    /* Store tens digits of the observed remainders. */
    uint8_t digits[200];
    int idx = 0;

    /* Detect cycle. */
    while (1) {
        if (rem_index[(int)r] != -1)    /* remainder repeated – cycle found */
            break;
        rem_index[(int)r] = idx;
        digits[idx] = (uint8_t)(r / 10ULL);   /* tens digit */
        ++idx;
        r = (r * base) % 100ULL;          /* next remainder */
    }

    int cycle_start = rem_index[(int)r];
    int cycle_len   = idx - cycle_start;

    /* Concatenate the cycle digits into the result. */
    seq_t result = (seq_t)0;
    for (int i = cycle_start; i < cycle_start + cycle_len; ++i) {
        result = result * (seq_t)10 + (seq_t)digits[i];
    }

    return result;
}
