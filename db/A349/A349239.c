/*
 * A349239  a(n) = n + (reversal of digits in the Zeckendorf representation of
 *          n).
 * https://oeis.org/A349239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n + reverse(zeckendorf digits of n)
 * Range:     u8 n <= 121, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the value of the OEIS sequence A349239.  */
seq_t a(idx_t n)
{
    /* Zeckendorf Fibonacci numbers: F[1]=1, F[2]=2, F[k]=F[k-1]+F[k-2] */
    size_t fib_cap = 8;
    seq_t *fib = (seq_t *)calloc(fib_cap, sizeof(seq_t));
    if (!fib) return (seq_t)0;   /* unlikely, but defensive */

    fib[1] = (seq_t)1;
    fib[2] = (seq_t)2;
    size_t fill_to = 3;          /* next position to fill, starts at 3 */

    /* Build Fibonacci numbers until a value would exceed n. */
    while (1) {
        seq_t next = fib[fill_to - 1] + fib[fill_to - 2];
        if (next > (seq_t)n) break;          /* no need to store the too‑large one */

        if (fill_to >= fib_cap) {             /* enlarge the array */
            fib_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(fib, fib_cap * sizeof(seq_t));
            if (!tmp) { free(fib); return (seq_t)0; }
            fib = tmp;
        }
        fib[fill_to] = next;
        ++fill_to;
    }

    /* Find the largest index L such that fib[L] <= n. */
    size_t L = 0;
    for (size_t i = 1; i < fill_to; ++i) {
        if (fib[i] <= (seq_t)n) L = i;
        else break;
    }

    /* Extract the Zeckendorf indices used (descending order). */
    /* At most L indices can be used. */
    idx_t *used = (idx_t *)calloc(L, sizeof(idx_t));
    if (!used) { free(fib); return (seq_t)0; }
    size_t used_cnt = 0;
    seq_t remaining = (seq_t)n;
    for (idx_t i = (idx_t)L; i > 0; --i) {
        if (fib[i] <= remaining) {
            used[used_cnt++] = i;
            remaining -= fib[i];
        }
    }

    /* Compute the reversed value: each used index j becomes
       index (L - j + 1) in the reversed number. */
    seq_t rev_val = (seq_t)0;
    for (size_t k = 0; k < used_cnt; ++k) {
        size_t shift = L - used[k] + 1;
        rev_val += fib[shift];
    }

    free(fib);
    free(used);

    return (seq_t)n + rev_val;
}
