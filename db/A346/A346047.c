/*
 * A346047  a(0) = a(1) = a(2) = 1; a(n) = Sum_{k=1..n-3} a(k) * a(n-k-3).
 * https://oeis.org/A346047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 28, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0)=a(1)=a(2)=1, a(3)=0,
 * a(n) = sum_{k=1..n-3} a(k) * a(n-k-3)   (n >= 4)
 */
#include <stddef.h>
#include <stdlib.h>

static seq_t *seq_cache = NULL;
static size_t seq_cap = 0;   /* allocated size of the cache */
static size_t seq_len = 0;   /* number of computed terms (index of first unused slot) */

static void seq_ensure(size_t n)
{
    if (n < seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap <= n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = tmp;
    seq_cap  = new_cap;
    /* initialise new slots to zero (necessary only for arbitrary‑precision types) */
    for (size_t i = seq_len; i < new_cap; ++i)
        seq_cache[i] = (seq_t)0;
}

static void seq_compute_to(size_t n)
{
    seq_ensure(n);
    for (size_t i = seq_len; i <= n; ++i) {
        if (i <= 2) {
            seq_cache[i] = (seq_t)1;
        } else if (i == 3) {
            seq_cache[i] = (seq_t)0;
        } else {
            seq_t sum = (seq_t)0;
            for (size_t k = 1; k <= i - 3; ++k) {
                sum += seq_cache[k] * seq_cache[i - k - 3];
            }
            seq_cache[i] = sum;
        }
    }
    seq_len = n + 1;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    if (N < seq_len)
        return seq_cache[N];
    seq_compute_to(N);
    return seq_cache[N];
}
