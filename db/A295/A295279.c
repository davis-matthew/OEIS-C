/*
 * A295279  Number of strict tree-factorizations of n.
 * https://oeis.org/A295279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 239, u16 n <= 4319, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A295279: Number of strict tree‑factorizations of n.
    Recurrence:  a(1)=1; for n>1,
        a(n) = 1 + Σ_{unordered factorisations of n into
                  two or more distinct integers >1}
                 ∏_{f∈F} a(f)

    The algorithm uses memoisation and a depth‑first search over
    increasing factor sequences, which guarantees each unordered
    factorisation is counted once.  The implementation runs in
    O(n·d(n)) time (where d(n) is the number of divisors), well
    below the 2‑second limit for n up to about 10 000.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Memoisation array --------------------------------------------------- */
static seq_t *memo = NULL;      /* cache of a(n) values */
static idx_t  memo_size = 0;    /* current capacity of memo */

/* Ensure the cache can hold values up to n. */
static void ensure_cache(idx_t n)
{
    if (n < memo_size) return;

    /* Grow to at least n+1.  A simple 2× growth keeps amortised cost O(1). */
    idx_t new_size = (memo_size == 0) ? n + 1 : memo_size;
    while (new_size <= n) new_size <<= 1;

    seq_t *new_mem = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_mem) return;            /* allocation failure – unlikely */

    /* copy old values */
    for (idx_t i = 0; i < memo_size; ++i) new_mem[i] = memo[i];
    for (idx_t i = memo_size; i < new_size; ++i) new_mem[i] = 0;

    free(memo);
    memo = new_mem;
    memo_size = new_size;
}

/* Forward declaration */
static seq_t compute_f(idx_t n);

/* Depth‑first enumeration of unordered factorizations of `rem`
   with factors strictly increasing, each > 1.
   `start` is the smallest allowed factor (previous factor + 1).
   `prod` accumulates the product of a(f) for already chosen factors.
   The recursion adds contributions directly to *total. */
static void dfs(idx_t rem, idx_t start, seq_t prod, idx_t depth, seq_t *total)
{
    /* iterate over divisors d of rem that are >= start and <= rem/d */
    for (idx_t d = start; d <= rem / d; ++d) {
        if (rem % d != 0) continue;

        /* choose factor d */
        seq_t next_prod = prod * compute_f(d);

        /* recurse on the remaining part rem/d, next factor must be > d */
        dfs(rem / d, d + 1, next_prod, depth + 1, total);

        /* no need for a complementary divisor here – it is handled
           as a separate branch when rem itself is taken as a factor.   */
    }

    /* Final factor: take rem itself (if allowed) */
    if (rem >= start && depth >= 1) {
        /* multiply by a(rem) */
        seq_t final_prod = prod * compute_f(rem);
        *total += final_prod;
    }
}

/* Compute a(n) with memoisation. */
static seq_t compute_f(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* a(1) = 1 by definition */

    ensure_cache(n);
    if (memo[n] != 0) return memo[n];     /* already computed */

    seq_t total = (seq_t)1;               /* case 1: the number itself */

    /* enumerate unordered factorizations of n */
    dfs(n, (idx_t)2, (seq_t)1, (idx_t)0, &total);

    memo[n] = total;
    return total;
}

/* External linkage function required by the ODE. */
seq_t a(idx_t n)
{
    return compute_f(n);
}
