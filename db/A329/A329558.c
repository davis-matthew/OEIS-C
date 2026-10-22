/*
 * A329558  Product of primes indexed by the first n squarefree numbers.
 * https://oeis.org/A329558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of primes whose indices are the first n
 *            squarefree numbers. a(0) = 1, a(1) = 2, a(2) = 6, a(3) = 30,
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_arr = NULL;
static size_t prime_arr_cnt = 0;
static idx_t prime_arr_needs = 0;

static void ensure_prime_arr(idx_t needed)
{
    if (prime_arr_needs >= needed) return;
    free(prime_arr);
    idx_t limit = _prime_limit_for_idx(needed);
    uint64_t *tmp = NULL;
    size_t cnt = 0;
    build_primes_upto(limit, &tmp, &cnt);
    prime_arr = tmp;
    prime_arr_cnt = cnt;
    prime_arr_needs = needed;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Estimate the index of the n-th squarefree number.
       Density of squarefree numbers is ≈ 6/π² ≈ 0.6079,
       so n-th squarefree ≤ ceil(n / 0.6) ≈ 2*n. */
    idx_t needed_prime_idx = (idx_t)(n * 2 + 12);
    ensure_prime_arr(needed_prime_idx);

    seq_t product = (seq_t)1;
    idx_t count = 0;
    idx_t x = 1;
    while (count < n) {
        if (is_squarefree((seq_t)x)) {
            /* prime indices are 1‑based, array is 0‑based */
            seq_t p = (seq_t)prime_arr[x - 1];
            product *= p;
            count++;
        }
        x++;
    }
    return product;
}
