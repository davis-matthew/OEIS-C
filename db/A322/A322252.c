/*
 * A322252  a(0) = 1 and a(n) = (5*n)!/(5!*n!^5) for n > 0.
 * https://oeis.org/A322252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5n)!/(5!*n!^5), a(0)=1, a(1)=1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compute exponent of prime p in n! */
static idx_t exp_in_factorial(idx_t n, unsigned int p)
{
    idx_t e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

/* Generate all primes ≤ limit, return array and count */
static void generate_primes(idx_t limit, unsigned int **out, size_t *cnt)
{
    size_t sz = (size_t)limit + 1;
    bool *is_composite = (bool *)calloc(sz, sizeof(bool));
    if (!is_composite) {
        *out = (unsigned int *)NULL;
        *cnt = 0;
        return;
    }
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!is_composite[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                is_composite[m] = true;
        }
    }
    size_t capacity = 10;
    size_t length = 0;
    unsigned int *arr = (unsigned int *)malloc(capacity * sizeof(unsigned int));
    if (!arr) {
        free(is_composite);
        *out = (unsigned int *)NULL;
        *cnt = 0;
        return;
    }
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            if (length == capacity) {
                capacity <<= 1;
                unsigned int *tmp = (unsigned int *)realloc(arr, capacity * sizeof(unsigned int));
                if (!tmp) {
                    free(arr);
                    free(is_composite);
                    *out = (unsigned int *)NULL;
                    *cnt = 0;
                    return;
                }
                arr = tmp;
            }
            arr[length++] = (unsigned int)i;
        }
    }
    free(is_composite);
    *out = arr;
    *cnt = length;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)1;

    /* Lazy initialization of prime list up to 5*n */
    static unsigned int *prime_list = NULL;
    static size_t prime_cnt   = 0;
    static idx_t   prime_lim   = 0;

    idx_t needed = 5 * n;
    if (needed > prime_lim) {
        unsigned int *new_list;
        size_t new_cnt;
        generate_primes((idx_t)needed, &new_list, &new_cnt);
        free(prime_list);
        prime_list = new_list;
        prime_cnt  = new_cnt;
        prime_lim  = needed;
    }

    seq_t result = (seq_t)1;

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        unsigned int p = prime_list[pi];
        if ((idx_t)p > needed) break;

        idx_t exp = exp_in_factorial(5 * n, p) - 5 * exp_in_factorial(n, p);
        if (p == 2) exp -= 3;   /* subtract exponent from 5! */
        if (p == 3) exp -= 1;
        if (p == 5) exp -= 1;
        if (exp == 0) continue;

        seq_t powp = (seq_t)1;
        for (idx_t i = 0; i < exp; ++i)
            powp *= (seq_t)p;

        result *= powp;
    }

    return result;
}
