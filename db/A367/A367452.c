/*
 * A367452  Number of semiprime divisors of the n-th squarefree number
 *          (A005117).
 * https://oeis.org/A367452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c*(c-1)/2 where c = number of distinct prime divisors of
 *            the n-th squarefree number (A005117(n))
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   count_distinct_primes, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"
#include "../utils/count_distinct_primes.h"

/* Cache for the generated squarefree numbers (uint64_t is enough for the limits considered) */
static uint64_t *squarefrees = NULL;   /* array of nth squarefree numbers (0‑based) */
static size_t sq_len = 0;              /* number of elements stored */
static size_t sq_cap = 0;              /* allocated capacity */

/* Ensure the cache can hold at least 'needed' elements */
static void ensure_sq_cap(size_t needed)
{
    if (needed <= sq_cap) return;
    size_t newcap = sq_cap ? sq_cap << 1 : 8;
    while (newcap < needed) newcap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(squarefrees, newcap * sizeof(uint64_t));
    if (!tmp) abort();                 /* allocation failure */
    squarefrees = tmp;
    sq_cap = newcap;
}

/* Generate squarefree numbers up to index (0‑based) */
static void generate_squarefrees(size_t upto)
{
    if (sq_len > upto) return;          /* already enough */
    ensure_sq_cap(upto + 1);
    uint64_t cand = (sq_len == 0) ? 1 : squarefrees[sq_len - 1] + 1;
    while (sq_len <= upto) {
        while (!is_squarefree((seq_t)cand)) cand++;
        squarefrees[sq_len++] = cand;
        cand++;
    }
}

/* Public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined before offset; safe fallback */
    size_t idx = (size_t)(n - 1);          /* convert to 0‑based index */
    generate_squarefrees(idx);

    uint64_t m = squarefrees[idx];
    size_t c = (size_t)count_distinct_primes(m);   /* m is squarefree, exponents are 1 */

    seq_t cc = (seq_t)c;
    return cc * (cc - 1) / 2;
}
