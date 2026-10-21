/*
 * A281861  Riordan transform of the Fibonacci sequence with the Riordan
 *          matrix A053121.
 * https://oeis.org/A281861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/(n+1)) * sum_{s=0}^{⌊n/2⌋} fib(n-2s) * (n-2s+1) *
 *            binom(n+1, s)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/fib.h"

static seq_t *fib_cache = NULL;
static idx_t fib_cache_len = 0;

/* Ensure that fib_cache contains Fibonacci numbers 0..n. */
static void ensure_fib_cache(idx_t n)
{
    if (fib_cache_len > n) return;
    idx_t new_len = (n + 1);
    fib_cache = (seq_t *)realloc(fib_cache, new_len * sizeof(seq_t));
    if (!fib_cache_len) {
        /* initialize first two Fibonacci numbers */
        fib_cache[0] = 0;
        if (new_len > 1) fib_cache[1] = 1;
        fib_cache_len = (new_len > 1) ? 2 : 1;
    }
    if (fib_cache_len < 2 && new_len > 1) {
        fib_cache[1] = 1;
        fib_cache_len = 2;
    }
    for (idx_t i = fib_cache_len; i <= n; ++i) {
        fib_cache[i] = fib_cache[i - 1] + fib_cache[i - 2];
    }
    fib_cache_len = new_len;
}

seq_t a(idx_t n)
{
    idx_t n1 = n + 1;
    seq_t sum = 0;
    /* Precompute Fibonacci numbers up to n */
    ensure_fib_cache(n);

    /* iterative computation of binom(n+1, s) */
    seq_t binom_val = 1;  /* binom(n+1, 0) */
    for (idx_t s = 0; s <= n / 2; ++s) {
        idx_t m = n - 2 * s;          /* m = n-2s */
        seq_t f = fib_cache[m];       /* fib(m) */

        /* term = f * (m+1) * binom_val */
        sum += f * ((seq_t)(m + 1)) * binom_val;

        /* update binom_val for next s */
        if (s == n / 2) break;
        /* binom(n+1, s+1) = binom(n+1, s) * (n+1-s) / (s+1) */
        seq_t num = n1 - s;           /* n+1-s */
        seq_t den = s + 1;            /* s+1 */
        binom_val = binom_val * num / den;
    }

    /* divide by n+1 (exact division) */
    return sum / (seq_t)n1;
}
