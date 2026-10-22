/*
 * A386324  Primes without {0, 6} as digits.
 * https://oeis.org/A386324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 3200, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_zero_or_six, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non-Formula: a(n) is the n‑th prime that does not contain the digits 0 or 6. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/has_zero_or_six.h"

static seq_t next_valid_prime(void)
{
    static seq_t cand = 2;
    for (;;)
    {
        if (cand == 2)
        {
            if (isPrimeGeneric(cand) && !has_zero_or_six(cand))
            {
                cand = 3;
                return 2;
            }
            cand = 3;
            continue;
        }

        if (isPrimeGeneric(cand) && !has_zero_or_six(cand))
        {
            seq_t result = cand;
            cand += 2;
            return result;
        }
        cand += 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    static seq_t *values = NULL;
    static size_t capacity = 0;
    static size_t length = 0;

    while (length < n)
    {
        seq_t next = next_valid_prime();
        if (length == capacity)
        {
            capacity = capacity ? capacity * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(values, capacity * sizeof(seq_t));
            values = tmp;
        }
        values[length++] = next;
    }
    return values[n - 1];
}
