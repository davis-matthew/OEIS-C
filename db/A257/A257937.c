/*
 * A257937  Primes p such that one digit appears exactly six times together
 *          with a single different digit.
 * https://oeis.org/A257937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes with exactly six identical digits and one distinct digit
   (OEIS A257937).  The set of qualifying numbers is fixed for 7‑digit
   integers, so we generate all candidates, test for primality, sort
   the primes once, and then answer queries from this table. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Static cache of primes once produced */
static seq_t primes[250];   /* 248 possible numbers, 250 is safe */
static size_t prime_count = 0;

/* Random access to the first primality table and sorting */
static void init_primes(void)
{
    if (prime_count != 0)                /* already initialised */
        return;

    const int repeated_digits[4] = {1, 3, 7, 9};
    const int total_positions   = 7;     /* 7‑digit numbers */

    for (int r = 0; r < 4; ++r) {
        int d = repeated_digits[r];      /* repeated digit */
        for (int e = 0; e <= 9; ++e) {   /* distinct digit */
            if (e == d) continue;        /* must differ from d */
            for (int pos = 0; pos < total_positions; ++pos) {
                /* Skip leading zero */
                if (pos == 0 && e == 0)
                    continue;

                /* Build the numeric value */
                uint64_t val = 0;
                int digits[7];
                for (int i = 0; i < total_positions; ++i)
                    digits[i] = d;
                digits[pos] = e;
                for (int i = 0; i < total_positions; ++i)
                    val = val * 10 + digits[i];

                seq_t v = (seq_t)val;      /* cast is safe for all builds */

                if (is_prime_big(v))
                    primes[prime_count++] = v;
            }
        }
    }

    /* Simple insertion sort – adequate for ≤250 items and works
       for both plain integer types and arbitrary‑precision classes. */
    for (size_t i = 0; i + 1 < prime_count; ++i) {
        for (size_t j = i + 1; j < prime_count; ++j) {
            if (primes[j] < primes[i]) {
                seq_t tmp = primes[i];
                primes[i] = primes[j];
                primes[j] = tmp;
            }
        }
    }
}

/* Public implementation of the OEIS sequence A257937 */
seq_t a(idx_t n)
{
    init_primes();               /* ensure table built */

    if (n == 0 || n > prime_count)
        return 0;                /* undefined for out‑of‑range indices */

    return primes[n - 1];
}
