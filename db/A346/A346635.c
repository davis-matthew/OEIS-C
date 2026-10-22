/*
 * A346635  Numbers whose division (or multiplication) by their greatest prime
 *          factor yields a perfect square. Numbers k such that k*A006530(k)
 *          is a perfect square.
 * https://oeis.org/A346635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = k such that k * gpf(k) is a perfect square
 * Range:     u8 n <= 6, u16 n <= 55, u32 n <= 6577, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, floor_sqrt, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: iterate k = 1,2,3,..., compute the largest prime factor of k,   */
/* multiply, test for perfect square, cache qualifying k; return the n-th cached k. */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x);            */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/seq_mul.h"     /* seq_t seq_mul(seq_t a, seq_t b);     */

/* Cached values                                                                  */
static seq_t *seq_vals = NULL;          /* array of qualifying k values            */
static size_t seq_cap = 0;              /* capacity of seq_vals                    */
static size_t seq_len = 0;              /* number of values stored                  */
static idx_t next_k = 1;                /* next integer to test                    */

/* ------------------------------------------------------------------------------ */
/* Small helper: largest prime factor of n (pf is 1 for n <= 1)                    */
static uint64_t lpf_of(uint64_t n)
{
    if (n <= 1) return 1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    uint64_t maxp = 1;
    for (size_t i = 0; i < cnt; ++i)
        if (primes[i] > maxp) maxp = primes[i];
    return maxp;
}

/* Ensure seq_vals has room for at least 'needed' elements                         */
static void ensure_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------------------ */
/* Return the n-th element of the sequence (OEIS offset 1).                         */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n == 0, guard clause   */

    /* Generate more terms until we have at least n of them.                       */
    while (seq_len < (size_t)n) {
        uint64_t k = next_k;
        uint64_t pf = lpf_of(k);
        seq_t prod = (seq_t)k * (seq_t)pf;
        seq_t root = floor_sqrt(prod);
        seq_t sq   = seq_mul(root, root);
        if (sq == prod) {              /* k * gpf(k) is a perfect square      */
            ensure_cap(seq_len + 1);
            seq_vals[seq_len++] = (seq_t)k;
        }
        ++next_k;
    }
    return seq_vals[n - 1];
}
