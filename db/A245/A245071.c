/*
 * A245071  a(n) = 12n - prime(n).
 * https://oeis.org/A245071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 12*n - prime(n)
 * Range:     i8 n <= 14, i16 n <= 7457, i32 n <= 7457, i64 n <= 7457,
 *            i128 n <= 7457, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


static uint64_t *prime_list = NULL;
static size_t prime_count   = 0;
static size_t prime_cap     = 0;
static uint64_t last_checked = 1;

/* Generate at least `needed` primes, storing them in prime_list.  */
static void ensure_primes(size_t needed)
{
    if (prime_count >= needed) return;

    /* Grow the buffer if necessary. */
    if (prime_cap < needed) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        prime_list = tmp;
        prime_cap  = new_cap;
    }

    /* Find more primes by trial division. */
    for (uint64_t num = last_checked + 1; prime_count < needed; ++num) {
        last_checked = num;
        bool is_prime = true;
        for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= num; ++i) {
            if (num % prime_list[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) prime_list[prime_count++] = num;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_primes((size_t)n);
    seq_t prime_n = (seq_t)prime_list[n - 1];
    seq_t result  = (seq_t)12 * (seq_t)n - prime_n;
    return result;
}
