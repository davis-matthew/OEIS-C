/*
 * A386604  Irregular triangle read by rows: T(n,k) is the number of even
 *          numbers in the k-th 2-dense sublist of divisors of n, with n >= 1,
 *          k >= 1.
 * https://oeis.org/A386604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 8756, u16 n <= 8756, u32 n <= 8756, u64 n <= 8756,
 *            u128 n <= 8756, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: For each n ≥ 1 enumerate its divisors in increasing order,
   split the list into maximal 2‑dense sublists (adjacent divisors d₁,d₂
   satisfy d₂ ≤ 2·d₁), count the even numbers in each sublist and output
   these counts in row‑major order.  The function a(n) therefore returns
   the nth count in this flattened triangle. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Global storage for the triangle values. */
static seq_t *seq   = NULL;   /* flattened triangle */
static size_t seq_len = 0;    /* number of elements stored */
static size_t seq_cap = 0;    /* capacity of the buffer */
static idx_t  next_row = 1;   /* next n to be processed */

/* Ensure the buffer has enough capacity for at least 'need' elements. */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap < need) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* out of memory */
        seq = tmp;
        seq_cap = new_cap;
    }
}

/* Comparison function for qsort on idx_t. */
static int cmp_idx(const void *a, const void *b)
{
    const idx_t *ia = (const idx_t *)a;
    const idx_t *ib = (const idx_t *)b;
    if (*ia < *ib) return -1;
    if (*ia > *ib) return 1;
    return 0;
}

/* Public function: return the nth term of sequence A386604. */
seq_t a(idx_t n)
{
    /* Iterate rows until we have produced at least n terms. */
    while (seq_len < n) {
        idx_t m = next_row++;
        /* Gather divisors of m. */
        idx_t divs[256];
        size_t dcnt = 0;
        for (idx_t i = 1; i * i <= m; ++i) {
            if (m % i == 0) {
                if (dcnt < sizeof(divs)/sizeof(divs[0]))
                    divs[dcnt++] = i;
                idx_t j = m / i;
                if (j != i && dcnt < sizeof(divs)/sizeof(divs[0]))
                    divs[dcnt++] = j;
            }
        }
        /* Sort divisors. */
        qsort(divs, dcnt, sizeof(idx_t), cmp_idx);

        /* Scan the sorted divisors, forming maximal 2‑dense sublists
           and counting the even numbers in each sublist. */
        seq_t evens = (divs[0] % 2 == 0) ? 1 : 0;
        for (size_t k = 1; k < dcnt; ++k) {
            if (divs[k] > divs[k-1] * (idx_t)2) {   /* start new sublist */
                ensure_seq_cap(seq_len + 1);
                seq[seq_len++] = evens;
                evens = (divs[k] % 2 == 0) ? 1 : 0;
            } else {
                if (divs[k] % 2 == 0) ++evens;
            }
        }
        /* Store the last sublist. */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len++] = evens;
    }
    return seq[n - 1];
}
