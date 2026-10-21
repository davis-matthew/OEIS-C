/*
 * A294236  Sum of the smaller parts of the partitions of n into two parts
 *          with larger part nonsquarefree.
 * https://oeis.org/A294236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..⌊n/2⌋} i * (1 - μ(n-i)^2) (= sum of the smaller
 *            parts of partitions of n into two parts where the larger part is
 *            not square‑free)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Smallest‑prime‑factor table, rebuilt to cover larger parts as needed. */
static idx_t *spf = NULL;
static idx_t spf_len = 0;

/* Create a complete smallest‑prime‑factor array up to “limit”. */
static void init_spf(idx_t limit)
{
    if (limit < 2) {
        if (spf) { free(spf); spf = NULL; }
        spf_len = 0;
        return;
    }
    if (spf_len > limit) return;            /* already sufficient */

    /* Allocate new array; cast to satisfy C++ name mangling. */
    idx_t *new_spf = (idx_t *)calloc(limit + 1, sizeof(idx_t));
    if (!new_spf) return;               /* allocation failure – keep old table */

    /* Sieve for smallest prime factor. */
    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {            /* i is prime */
            new_spf[i] = i;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0) new_spf[j] = i;
            }
        }
    }

    /* Replace old table. */
    if (spf) free(spf);
    spf = new_spf;
    spf_len = limit;
}

/* Test whether a number is square‑free using the SPF table. */
static bool is_squarefree(idx_t n)
{
    if (n <= 1) return true;           /* 0 and 1 are conventionally square‑free */

    idx_t x = n;
    while (x > 1) {
        idx_t p = spf[x];
        if (p == 0) p = x;             /* fall back to the number itself if absent */
        idx_t exp = 0;
        while (x % p == 0) {
            ++exp;
            x /= p;
            if (exp >= 2) return false; /* repeated prime factor → not square‑free */
        }
    }
    return true;
}

/* Compute the sequence value a(n). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* defined only for n ≥ 1 */

    init_spf(n);                       /* ensure SPF table covers all larger parts */

    idx_t half = n / 2;
    seq_t sum = 0;

    for (idx_t i = 1; i <= half; ++i) {
        idx_t d = n - i;               /* larger part of the partition */
        if (!is_squarefree(d)) {
            sum += (seq_t)i;           /* add the smaller part only when the larger part is not square‑free */
        }
    }
    return sum;
}
