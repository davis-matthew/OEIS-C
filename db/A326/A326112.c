/*
 * A326112  Numbers requiring 3 central factorizations to reach prime
 *          factorization.
 * https://oeis.org/A326112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer m (m ≥ 2) for which the number of
 *            successive central factorizations needed to reach the prime
 *            factorization of m equals 3
 * Range:     u8 n <= 4, u16 n <= 3563, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A326112: Numbers requiring 3 central factorizations to reach prime factorization. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/floor_sqrt.h"

static int depth_seq(seq_t n);

static int depth_seq(seq_t n)
{
    /* depth(n) = 0 if n is prime.
       Otherwise depth(n) = 1 + max( depth(a), depth(b) )
       where a*b is the central factorization of n. */
    if (is_prime_big(n))
        return 0;

    seq_t s = floor_sqrt(n);                /* floor(sqrt(n)) */
    seq_t d = s;

    /* Find largest divisor d <= sqrt(n) with d > 1 and n % d == 0 */
    while (d >= (seq_t)2) {
        if ((n % d) == (seq_t)0)
            break;
        d = d - (seq_t)1;
    }
    seq_t a = d;
    seq_t b = n / d;

    int da = depth_seq(a);
    int db = depth_seq(b);
    return 1 + (da > db ? da : db);
}

static seq_t *terms = NULL;           /* array of computed a(n) values (1‑based) */
static size_t terms_len = 0;          /* number of elements stored */
static size_t terms_cap = 0;          /* allocated capacity of the array */
static idx_t next_check = 2;          /* next integer to test for depth = 3 */

seq_t a(idx_t n)
{
    /* Ensure the array is large enough to store at least n terms. */
    if (terms_cap == 0) {
        terms_cap = 128;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        terms_len = 0;
        next_check = 2;
    }

    while (terms_len < (size_t)n) {
        seq_t m = (seq_t)next_check;
        if (depth_seq(m) == 3) {
            if (terms_len == terms_cap) {
                terms_cap <<= 1;                 /* grow capacity */
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = m;
        }
        next_check = (idx_t)((seq_t)next_check + (seq_t)1);
    }

    return terms[n - 1];
}
