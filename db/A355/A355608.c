/*
 * A355608  Zeroless numbers k such that x^2 - s*x + p has only integer roots,
 *          where s and p denote the sum and product of the digits of k
 *          respectively.
 * https://oeis.org/A355608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 976, u32 n <= 3366, u64 n <= 3366,
 *            u128 n <= 3366, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: zeroless numbers k such that the quadratic x^2 - s*x + p
   (s = sum of digits of k, p = product of digits of k) has only
   integer roots.  The terms are generated in increasing order by
   testing consecutive integers and memoising the valid ones. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* integer square root of a 64‑bit unsigned integer */
static inline uint64_t sqrt_u64(uint64_t x)
{
    uint64_t result = 0;
    uint64_t bit = 1ULL << 62;          /* highest power of four <= 2^64-1 */
    while (bit > x) bit >>= 2;
    while (bit) {
        uint64_t t = result + bit;
        if (x >= t) {
            x -= t;
            result = (result >> 1) + bit;
        } else {
            result >>= 1;
        }
        bit >>= 2;
    }
    return result;
}

/* static storage for memoised terms */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static uint64_t next_check = 1;   /* next integer to test */

/* append a term, growing the array if necessary */
static void append_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 256;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* generate terms until we have at least 'target' of them */
static void generate_up_to(idx_t target)
{
    while (terms_len < target) {
        uint64_t k = next_check++;
        uint64_t t   = k;
        uint64_t sum = 0;
        uint64_t prod = 1;
        bool has_zero = false;

        while (t > 0) {
            uint64_t d = t % 10;
            if (d == 0) { has_zero = true; break; }
            sum += d;
            prod *= d;
            t /= 10;
        }
        if (has_zero) continue;                /* not zeroless */

        uint64_t disc  = sum * sum;            /* s^2 */
        uint64_t fourp = prod * 4;             /* 4p */
        if (disc < fourp) continue;

        uint64_t D   = disc - fourp;
        uint64_t sq  = sqrt_u64(D);
        if (sq * sq != D) continue;           /* discriminant not a square */

        if ((sum ^ sq) & 1) continue;         /* parity mismatch */

        append_term((seq_t)k);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined; safeguard */
    if (!terms) {                              /* lazy initialisation */
        terms_cap = 256;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }
    if (terms_len < n) generate_up_to(n);
    return terms[n - 1];
}
