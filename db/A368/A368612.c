/*
 * A368612  a(n) = Sum_{k=2..n} pi(k-1) * ceiling(n/k).
 * https://oeis.org/A368612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=2..n} pi(k-1) * ceil(n/k)
 * Range:     u8 n <= 23, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Allocate or resize a prime sieve and pi array up to `limit` */
static void up_to(idx_t limit, uint8_t **prime_flag, idx_t **pi_arr, idx_t *max_limit)
{
    if (*max_limit >= limit)
        return;

    /* free old data */
    free(*prime_flag);
    free(*pi_arr);

    /* allocate new arrays */
    *prime_flag = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    *pi_arr     = (idx_t  *)calloc(limit + 1, sizeof(idx_t));
    *max_limit  = limit;

    if (*prime_flag == NULL || *pi_arr == NULL) {
        /* allocation failed; abort */
        exit(EXIT_FAILURE);
    }

    /* mark 0 and 1 as non-prime */
    (*prime_flag)[0] = (*prime_flag)[1] = 1;

    /* simple sieve */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if ((*prime_flag)[i] == 0) {          /* i is prime */
            for (idx_t j = i * i; j <= limit; j += i)
                (*prime_flag)[j] = 1;
        }
    }

    /* build cumulative pi array */
    idx_t count = 0;
    for (idx_t i = 0; i <= limit; ++i) {
        if ((*prime_flag)[i] == 0 && i >= 2)
            ++count;
        (*pi_arr)[i] = count;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    static uint8_t *prime_flag = NULL;
    static idx_t  *pi_arr     = NULL;
    static idx_t   max_limit  = 0;

    up_to(n, &prime_flag, &pi_arr, &max_limit);

    seq_t sum = 0;
    for (idx_t k = 2; k <= n; ++k) {
        seq_t pi_val  = (seq_t)pi_arr[k - 1];
        seq_t ceil_val = (seq_t)((n + k - 1) / k);  /* ceil(n/k) */
        sum += pi_val * ceil_val;
    }
    return sum;
}
