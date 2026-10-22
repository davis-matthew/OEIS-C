/*
 * A342259  Numbers k such that omega(k-1) = omega(k)-1 and omega(k+1) =
 *          omega(k)+1, where omega(m) is the number of distinct primes
 *          dividing m, A001221(m).
 * https://oeis.org/A342259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers k with ω(k‑1)=ω(k)‑1 and ω(k+1)=ω(k)+1, ω = ω(n)
 *            = # of distinct prime factors
 * Range:     u8 n <= 5, u16 n <= 1722, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/count_distinct_primes.h"

/* Return true if k satisfies the definition. */
static bool is_good(idx_t k)
{
    if (k < 2) return false;                /* k=1 gives k‑1=0, not handled by count_distinct_primes */
    uint64_t k64 = (uint64_t)k;
    uint64_t km1 = k64 - 1;
    uint64_t kp1 = k64 + 1;

    int wkm1 = count_distinct_primes(km1);
    int wk   = count_distinct_primes(k64);
    int wkp1 = count_distinct_primes(kp1);

    return (wkm1 == wk - 1) && (wkp1 == wk + 1);
}

/* Dynamic array holding discovered terms. */
static seq_t *terms = NULL;        /* array of terms */
static size_t t_len = 0;           /* number of stored terms */
static size_t t_cap = 0;           /* array capacity */
static idx_t last_chk = 1;         /* largest candidate that has been tested */

/* Append a new term to the dynamic array. */
static void append_term(seq_t val)
{
    if (t_cap == 0) {
        t_cap = 8;
        terms = (seq_t *)malloc(t_cap * sizeof(seq_t));
    } else if (t_len == t_cap) {
        t_cap <<= 1;
        terms = (seq_t *)realloc(terms, t_cap * sizeof(seq_t));
    }
    terms[t_len++] = val;
}

/* External function required by the problem statement. */
seq_t a(idx_t n)
{
    /* Extend the known sequence until it contains n terms. */
    while (t_len < n) {
        last_chk++;                     /* next integer to test */
        if (is_good(last_chk))
            append_term((seq_t)last_chk); /* store value after truncation to seq_t */
    }
    return terms[n - 1];                /* 0‑based index */
}
