/*
 * A279455  Numbers n such that the number of nonprime divisors of n divides
 *          n.
 * https://oeis.org/A279455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th integer such that (tau(n) – omega(n)) divides n
 *            (tau = number of divisors, omega = number of distinct prime
 *            factors)
 * Range:     u8 n <= 132, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Check whether a given integer n satisfies the condition:
   (tau(n) – omega(n)) divides n.  The function uses only 64‑bit
   arithmetic; the final result is cast to seq_t. */
static bool qualifies(idx_t n)
{
    uint64_t m = (uint64_t)n;
    uint64_t tau = 1;      /* divisor count accumulator */
    uint64_t omega = 0;    /* distinct prime factor counter */

    /* factor 2 separately */
    if ((m & 1) == 0) {
        omega++;
        uint64_t exp = 0;
        do {
            m >>= 1;
            exp++;
        } while ((m & 1) == 0);
        tau *= (exp + 1);
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            omega++;
            uint64_t exp = 0;
            do {
                m /= p;
                exp++;
            } while (m % p == 0);
            tau *= (exp + 1);
        }
    }

    /* any remaining factor is a prime */
    if (m > 1) {
        omega++;
        tau *= 2;
    }

    uint64_t nonprime = tau - omega;
    if (nonprime == 0) return false;
    return (n % nonprime) == 0;
}

/* ----------  static storage for the computed sequence  ---------- */
static seq_t *seq_array = NULL;   /* array of stored terms      */
static size_t seq_len = 0;        /* number of terms stored     */
static size_t seq_cap = 0;        /* allocated capacity         */
static uint64_t next_candidate = 1;   /* next integer to test      */

/* Append a new term to the static array, extending capacity as needed */
static void append_term(idx_t value)
{
    if (seq_len == seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 16 : seq_cap << 1;
        seq_t *tmp = (seq_t *)realloc(seq_array, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* allocation failure */
        seq_array = tmp;
        seq_cap = new_cap;
    }
    seq_array[seq_len++] = (seq_t)value;
}

/* ----------  public API  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1, harmless               */

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        if (qualifies(next_candidate))
            append_term((idx_t)next_candidate);
        next_candidate++;
    }

    return seq_array[(size_t)n - 1];
}
