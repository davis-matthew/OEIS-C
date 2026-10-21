/*
 * A282846  Start with a(0) = 0, c = 1, p = 2. Thereafter, if a(n) < c, then
 *          a(n+1) = a(n) + p and p is set to the next larger prime; else,
 *          a(n+1) = a(n) - c and c is set to the next larger composite
 *          number.
 * https://oeis.org/A282846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A282846:
   a(0)=0, c=1, p=2.
   For n>=0:
     if a(n) < c : a(n+1)=a(n)+p and p:=next prime after p
     else         : a(n+1)=a(n)-c and c:=next composite after c
   (All values are non‑negative integers.) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* small prime test (trial division) */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0 || (n % 3) == 0) return false;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

/* next prime > n */
static uint64_t next_prime(uint64_t n)
{
    uint64_t x = n + 1;
    while (!is_prime_u64(x)) ++x;
    return x;
}

/* next composite > n (composite means >1 and not prime) */
static uint64_t next_composite(uint64_t n)
{
    uint64_t x = n + 1;
    while (x <= 1 || is_prime_u64(x)) ++x;
    return x;
}

/* Compute A282846 */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;          /* number of computed terms */
    static uint64_t c = 1;             /* current composite threshold */
    static uint64_t p = 2;             /* current prime for addition */

    /* initialise */
    if (seq_len == 0) {
        seq_cap = 128;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)0;
        seq_len = 1;
    }

    /* grow capacity if needed */
    if ((size_t)n + 1 > seq_cap) {
        size_t new_cap = seq_cap;
        while ((size_t)n + 1 > new_cap) new_cap *= 2;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* extend sequence up to n */
    if ((size_t)n + 1 > seq_len) {
        for (size_t i = seq_len; i <= (size_t)n; ++i) {
            seq_t prev = seq[i - 1];
            if (prev < (seq_t)c) {
                seq[i] = prev + (seq_t)p;
                p = next_prime(p);
            } else {
                seq[i] = prev - (seq_t)c;
                c = next_composite(c);
            }
        }
        seq_len = (size_t)n + 1;
    }

    return seq[(size_t)n];
}
