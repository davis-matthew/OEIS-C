/*
 * A300654  a(n) is the greatest k such that, for i = 1..k, the binary
 *          representation of i appears as a substring in the binary
 *          representation of 1/n (ignoring the radix point and adding
 *          trailing zeros if necessary in case of a terminating expansion).
 * https://oeis.org/A300654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) as the greatest k such that each binary representation of
   i = 1..k appears as a substring of the binary expansion of 1/n.
   The function builds the expansion, constructs presence tables for
   all binary patterns up to MAXLEN bits, and scans for the largest
   k present entirely. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static int bitlen_uint64(uint64_t x) {
    int l = 0;
    while (x) { l++; x >>= 1; }
    return l;
}

#define MAXLEN 20          // maximum pattern length we recognize
#define REPCOUNT 20        // repetition count for periodic parts

seq_t a(idx_t n) {
    /* Special case: n == 1 (1/1 = 1.0) */
    if (n == 1) return (seq_t)2;

    /* Find preperiod and period of the binary expansion of 1/n. */
    int *first = (int *)calloc((size_t)n, sizeof(int));
    if (!first) return (seq_t)0;          // allocation failure guard
    for (size_t i = 0; i < (size_t)n; ++i) first[i] = -1;

    /* Collect bits until a remainder repeats or becomes 0. */
    size_t bits_cap = n * 2 + 10;          // safe capacity
    char *bits = (char *)malloc(bits_cap);
    if (!bits) { free(first); return (seq_t)0; }
    size_t bits_len = 0;
    int r = 1 % (int)n;                    // initial remainder for numerator 1

    int preperiod = 0, period = 0;
    while (r != 0 && first[r] == -1) {
        first[r] = (int)bits_len;
        long long tmp = (long long)r * 2;
        int bit = tmp / (int)n;
        r = (int)(tmp % n);
        if (bits_len == bits_cap) {
            bits_cap <<= 1;
            bits = (char *)realloc(bits, bits_cap);
            if (!bits) { free(first); return (seq_t)0; }
        }
        bits[bits_len++] = (char)bit;
    }
    if (r == 0) {                 // terminating expansion
        preperiod = (int)bits_len;
        period = 0;
    } else {                      // repeating expansion
        preperiod = first[r];
        period = (int)bits_len - preperiod;
    }
    free(first);

    /* Build a sufficiently long bit string that covers any pattern
       up to MAXLEN bits. For periodic expansions we repeat the cycle
       REPCOUNT times; for terminating expansions we append MAXLEN zeros. */
    size_t tot_len = (size_t)preperiod + (size_t)period * REPCOUNT
                     + (period == 0 ? MAXLEN : 0);
    char *all = (char *)malloc(tot_len);
    if (!all) { free(bits); return (seq_t)0; }

    size_t pos = 0;
    /* Copy preperiod bits */
    for (size_t i = 0; i < (size_t)preperiod; ++i) {
        all[pos++] = bits[i];
    }
    if (period > 0) {
        /* Repeat the repeating block */
        for (int j = 0; j < REPCOUNT; ++j) {
            memcpy(all + pos, bits + preperiod, (size_t)period);
            pos += (size_t)period;
        }
    } else {
        /* Append zeros after the terminating part */
        for (size_t i = pos; i < tot_len; ++i) {
            all[i] = 0;
        }
    }
    free(bits);

    /* Precompute presence of all binary patterns up to MAXLEN bits. */
    char *present[MAXLEN + 1];
    for (int l = 1; l <= MAXLEN; ++l) {
        size_t sz = ((size_t)1) << l;
        present[l] = (char *)calloc(sz, 1);
    }

    for (int l = 1; l <= MAXLEN; ++l) {
        if (tot_len < (size_t)l) continue;   /* window too short */
        unsigned int val = 0;
        /* first window */
        for (int i = 0; i < l; ++i) {
            val = (val << 1) | (unsigned int)all[i];
        }
        present[l][val] = 1;
        unsigned int mask = ((unsigned int)1 << l) - 1;
        for (size_t i = l; i < tot_len; ++i) {
            val = ((val << 1) & mask) | (unsigned int)all[i];
            present[l][val] = 1;
        }
    }

    /* Search for the largest k whose binary pattern occurs. */
    uint64_t k = 0;
    for (uint64_t i = 1; ; ++i) {
        int l = bitlen_uint64(i);
        if (l > MAXLEN) break;                 /* beyond our tables */
        if (!present[l][(unsigned int)i]) break;
        k = i;
    }

    /* Clean up */
    for (int l = 1; l <= MAXLEN; ++l) free(present[l]);
    free(all);

    return (seq_t)k;
}
