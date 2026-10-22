/*
 * A328828  Index of the least significant digit larger than one in the
 *          primorial base expansion of n, 0 if no such digit exists.
 * https://oeis.org/A328828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = index (1‑based) of the least significant digit larger *
 *            than one in the primorial base expansion of n, or 0 if no such
 *            digit * exists. Primorial base uses radices 2,3,5,7,11,… (the
 *            successive primes)
 * Range:     u8 n <= 9676, u16 n <= 9676, u32 n <= 9676, u64 n <= 9676,
 *            u128 n <= 9676, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- simple prime table ---------- */
static idx_t *prime_arr = NULL;   /* stored primes */
static size_t prime_cnt = 0;      /* number of stored primes */
static size_t prime_cap = 0;      /* allocated capacity */

/* append a new prime to the table */
static void prime_append(idx_t p)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 8;
        idx_t *tmp = (idx_t *)realloc(prime_arr, new_cap * sizeof(idx_t));
        if (!tmp) abort();          /* should not happen */
        prime_arr = tmp;
        prime_cap = new_cap;
    }
    prime_arr[prime_cnt++] = p;
}

/* check primality by trial division using stored primes */
static bool prime_is_prime(idx_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_arr[i];
        if (p > n / p) break;     /* p*p > n */
        if (n % p == 0) return false;
    }
    return true;
}

/* ensure that the table contains at least pos+1 primes (0‑based) */
static void prime_ensure(size_t pos)
{
    while (prime_cnt <= pos) {
        idx_t candidate = prime_cnt ? prime_arr[prime_cnt-1] + 1 : 2;
        /* skip even numbers after 2 */
        if (candidate > 2 && (candidate & 1) == 0) candidate++;
        for (;;) {
            if (prime_is_prime(candidate)) {
                prime_append(candidate);
                break;
            }
            candidate += (candidate & 1) ? 2 : 1;   /* next odd */
        }
    }
}

/* return the k‑th prime (0‑based) */
static idx_t prime_get(size_t k)
{
    prime_ensure(k);
    return prime_arr[k];
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t r = n;      /* working copy of the number */
    size_t k = 0;     /* digit position (0‑based, least significant) */

    while (1) {
        idx_t p = prime_get(k);      /* radix at this position */
        idx_t digit = r % p;         /* current digit */
        if (digit > (idx_t)1)        /* first digit > 1 found */
            return (seq_t)(k + 1);   /* convert to 1‑based index */

        r = r / p;                   /* shift to next more significant digit */
        if (r == 0) break;           /* no further digits */
        ++k;
    }
    return (seq_t)0;                  /* all digits ≤ 1 */
}
