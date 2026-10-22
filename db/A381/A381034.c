/*
 * A381034  Numbers that have a digit > 1 in their primorial base expansion,
 *          and that are multiples of the corresponding radix prime of the
 *          least significant such digit.
 * https://oeis.org/A381034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   enumerate positive integers and select those whose primorial *
 *            base expansion has a least significant digit > 1 that
 *            corresponds to a * prime divisor of the integer. This implements
 *            A381034
 * Range:     u8 n <= 31, u16 n <= 8855, u32 n <= 8856, u64 n <= 8856,
 *            u128 n <= 8856, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Prime generator --------------------------------------------------------- */
static uint64_t *prime_list = NULL;   /* Array of generated primes     */
static size_t   prime_count = 0;     /* Number of primes stored       */
static size_t   prime_cap   = 0;     /* Allocated capacity of array   */

/* Generate the next prime and append it to prime_list.  */
static void
generate_next_prime (void)
{
    if (prime_count == 0) {                     /* first prime */
        prime_cap   = 8;
        prime_list  = (uint64_t *)malloc (prime_cap * sizeof (uint64_t));
        prime_list[0] = 2;
        prime_count   = 1;
        return;
    }

    uint64_t candidate = prime_list[prime_count - 1] + 1;
    for (;; ++candidate) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t q = prime_list[i];
            if (q > candidate / q) break; /* q*q would overflow or > sqrt(candidate) */
            if (candidate % q == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_count == prime_cap) {
                prime_cap   = (prime_cap == 0) ? 8 : prime_cap * 2;
                prime_list  = (uint64_t *)realloc (prime_list,
                                                   prime_cap * sizeof (uint64_t));
            }
            prime_list[prime_count++] = candidate;
            break;
        }
    }
}

/* Return the prime having index 'idx' (0‑based); primes are generated on demand. */
static uint64_t
get_prime_by_index (size_t idx)
{
    while (prime_count <= idx) {
        generate_next_prime ();
    }
    return prime_list[idx];
}

/* Test whether an integer n satisfies the condition for A381034.  */
static bool
qualifies (seq_t n)
{
    seq_t m = n;
    size_t prime_idx = 0;

    while (m != 0) {
        uint64_t p = get_prime_by_index (prime_idx);
        seq_t digit = m % (seq_t)p;

        if (digit > (seq_t)1) {
            return (n % (seq_t)p) == (seq_t)0;
        }

        m /= (seq_t)p;
        ++prime_idx;
    }
    return false; /* no digit > 1 found */
}

/* Return the nth term of A381034.  */
seq_t
a (idx_t n)
{
    idx_t   count  = 0;
    seq_t   value  = 1;

    while (true) {
        if (qualifies (value)) {
            ++count;
            if (count == n)
                return value;
        }
        ++value;
    }
}
