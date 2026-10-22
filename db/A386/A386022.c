/*
 * A386022  Primes having only {0, 1, 2, 9} as digits.
 * https://oeis.org/A386022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Computes the nth prime whose decimal digits are only 0,1,2,9 by
 *            brute‑force enumeration
 * Range:     u8 n <= 10, u16 n <= 103, u32 n <= 46, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

/* Static cache for the primes found so far */
static seq_t *prime_arr = NULL;   /* array of primes */
static size_t prime_cap = 0;      /* current capacity of prime_arr   */
static size_t prime_len = 0;      /* number of stored primes         */
static size_t processed_len = 0;  /* length of numbers processed     */

/* Helper: add a prime to the cache, resizing if necessary */
static void add_prime(seq_t p)
{
    if (prime_len == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(prime_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        prime_arr = tmp;
        prime_cap = new_cap;
    }
    prime_arr[prime_len++] = p;
}

/* Recursively generate numbers with the given length, starting from 0 */
static void gen_rec(int pos, int len, seq_t cur)
{
    if (pos == len) {
        if (is_prime_big(cur))
            add_prime(cur);
        return;
    }

    static const int digs[4] = {0, 1, 2, 9};
    for (int i = 0; i < 4; ++i) {
        int d = digs[i];
        if (pos == 0 && d == 0) continue;   /* no leading zero */
        seq_t next = cur * 10 + d;
        gen_rec(pos + 1, len, next);
    }
}

/* Generate all primes consisting of digits {0,1,2,9} with exactly len digits */
static void generate_length(int len)
{
    gen_rec(0, len, 0);
}

/* Public function required by the harness */
seq_t a(idx_t n)
{
    if (n == 0) abort();                     /* no a(0) */

    /* Return cached value if we already have it */
    if (prime_len >= (size_t)n)
        return prime_arr[n - 1];

    /* Generate additional primes until we reach the required index */
    while (prime_len < (size_t)n) {
        ++processed_len;
        generate_length((int)processed_len);
    }

    return prime_arr[n - 1];
}
