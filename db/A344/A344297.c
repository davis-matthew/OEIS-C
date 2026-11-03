/*
 * A344297  Heinz numbers of integer partitions of even numbers with no part
 *          greater than 3.
 * https://oeis.org/A344297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Heinz numbers of integer partitions of even numbers with no part greater than 3.
   The numbers are 2^a * 3^b * 5^c with non‑negative a,b,c satisfying a ≡ c (mod 2),
   sorted in increasing order.  This implementation builds the sequence by
   generating all 2,3,5‑smooth (Hamming) numbers and retaining only those that
   satisfy the parity condition. */

/* Include standard headers for memory management and integer types. */
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper to compute parity of exponent of 2 and 5 in a number. */
static void parity_of_two_and_five(seq_t n, int *p2, int *p5)
{
    seq_t tmp = n;
    int e2 = 0;
    while (tmp % (seq_t)2 == (seq_t)0) {
        tmp /= (seq_t)2;
        e2++;
    }
    *p2 = e2 & 1;

    tmp = n;
    int e5 = 0;
    while (tmp % (seq_t)5 == (seq_t)0) {
        tmp /= (seq_t)5;
        e5++;
    }
    *p5 = e5 & 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static state for memoization. */
    static seq_t *hamming   = NULL;
    static size_t h_cap = 0, h_len = 0;
    static idx_t idx2 = 0, idx3 = 0, idx5 = 0;

    static seq_t *good     = NULL;
    static size_t g_cap = 0, g_len = 0;

    /* ----- Initialization ----- */
    if (h_len == 0) {
        h_cap = 64;
        hamming = mem_new(seq_t, h_cap);
        if (!hamming) return (seq_t)0;  /* safety */
        hamming[0] = (seq_t)1;
        h_len = 1;
    }
    if (g_len == 0) {
        g_cap = 64;
        good = mem_new(seq_t, g_cap);
        if (!good) return (seq_t)0;  /* safety */
        good[0] = (seq_t)1;         /* the empty partition gives 1 */
        g_len = 1;
    }

    /* Generate until we have at least n good numbers. */
    while (g_len < n) {
        /* Compute next Hamming number by the three‑pointer method. */
        seq_t v2 = hamming[idx2] * (seq_t)2;
        seq_t v3 = hamming[idx3] * (seq_t)3;
        seq_t v5 = hamming[idx5] * (seq_t)5;
        seq_t next = v2;
        if (v3 < next) next = v3;
        if (v5 < next) next = v5;

        if (v2 == next) idx2++;
        if (v3 == next) idx3++;
        if (v5 == next) idx5++;

        /* Grow Hamming array if needed. */
        if (h_len == h_cap) {
            size_t new_cap = h_cap << 1;
            seq_t *tmp = mem_resize(seq_t, hamming, new_cap);
            if (!tmp) return (seq_t)0;
            hamming = tmp;
            h_cap = new_cap;
        }
        hamming[h_len++] = next;

        /* Check the parity condition a ≡ c (mod 2). */
        int p2, p5;
        parity_of_two_and_five(next, &p2, &p5);
        if (p2 == p5) {
            /* Add to good array. */
            if (g_len == g_cap) {
                size_t new_cap = g_cap << 1;
                seq_t *tmp = mem_resize(seq_t, good, new_cap);
                if (!tmp) return (seq_t)0;
                good = tmp;
                g_cap = new_cap;
            }
            good[g_len++] = next;
        }
    }

    /* Return the requested term.  Indexing is 1‑based as required. */
    return good[n - 1];
}
