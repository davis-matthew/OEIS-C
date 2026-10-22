/*
 * A374223  Numbers k such that sigma(k) and sopfr(k) are both multiples of 3,
 *          where sigma is the sum of divisors, and sopfr is the sum of prime
 *          factors with repetition.
 * https://oeis.org/A374223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1810, u32 n <= 9500, u64 n <= 9500,
 *            u128 n <= 9500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A374223
   Numbers k such that sigma(k) and sopfr(k) are both multiples of 3.
   The code generates the sequence by testing integers one by one,
   recording those that satisfy the condition, and returning the nth
   stored element. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Check whether a given unsigned integer k satisfies the condition
   that sigma(k) and sopfr(k) are multiples of 3.  */
static bool ok(uint64_t k)
{
    seq_t sigma = 1;
    seq_t sopfr = 0;
    uint64_t n = k;

    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            /* term = (p^(e+1) - 1) / (p - 1) */
            seq_t pp = 1;                /* p^e */
            for (int i = 0; i < e; ++i) {
                pp *= (seq_t)p;
            }
            seq_t p_pow = pp * (seq_t)p; /* p^(e+1) */
            seq_t denom = (seq_t)p - 1;
            seq_t term = (p_pow - 1) / denom;
            sigma *= term;
            sopfr += (seq_t)p * e;
        }
    }
    if (n > 1) {
        /* Remaining prime factor (exponent 1) */
        uint64_t p = n;
        seq_t pp = (seq_t)p;
        seq_t p_pow = pp * pp;          /* p^2 */
        seq_t denom = (seq_t)p - 1;
        seq_t term = (p_pow - 1) / denom; /* equals p + 1 */
        sigma *= term;
        sopfr += (seq_t)p;
    }
    return ((sigma % (seq_t)3) == 0) && ((sopfr % (seq_t)3) == 0);
}

/* Return the nth term of the sequence (1‑based).  */
seq_t a(idx_t n)
{
    static seq_t *values = NULL;   /* array of found terms */
    static size_t cap = 0, size = 0;
    static uint64_t next_candidate = 1;   /* next integer to test */

    while (size < n) {
        uint64_t k = next_candidate++;
        if (ok(k)) {
            if (size == cap) {
                size_t newcap = (cap == 0) ? 4 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(values, newcap * sizeof(seq_t));
                if (!tmp) abort();      /* allocation failure */
                values = tmp;
                cap = newcap;
            }
            values[size++] = (seq_t)k;
        }
    }
    return values[n - 1];
}
