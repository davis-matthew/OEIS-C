/*
 * A322795  Number of integers k, 0 <= k <= n, such that the
 *          Damerau-Levenshtein distance between the binary representations of
 *          n and k is strictly less than the Levenshtein distance.
 * https://oeis.org/A322795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of integers k, 0 <= k <= n, such that the
 *            Damerau–Levenshtein distance between the binary representations
 *            of n and k is strictly less than the Levenshtein distance
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   damerau_levenshtein
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/damerau_levenshtein.h"

/* Helper: get the binary representation of x.
   Bits are stored in buf[0..len-1] with the most
   significant bit first. The function returns len. */
static unsigned get_bits(idx_t x, uint8_t *buf) {
    if (x == (idx_t)0) {
        buf[0] = 0;
        return 1;
    }
    unsigned len = 0;
    idx_t tmp = x;
    while (tmp) {          /* determine bit length */
        len++;
        tmp >>= 1;
    }
    for (unsigned i = 0; i < len; ++i) {
        unsigned shift = len - 1 - i;
        buf[i] = (x >> shift) & 1U;
    }
    return len;
}

/* Levenshtein distance between two binary strings a[0..len_a-1] and
   b[0..len_b-1]. The arrays must have – at least – len+1 rows
   and columns.  The result fits into a small integer. */
static int levenshtein(const uint8_t *a, unsigned len_a,
                       const uint8_t *b, unsigned len_b)
{
    /* maximum length is small (<= 64 for all n <= 10000)      */
    static idx_t dp[70][70];

    for (unsigned i = 0; i <= len_a; ++i)
        dp[i][0] = i;
    for (unsigned j = 0; j <= len_b; ++j)
        dp[0][j] = j;

    for (unsigned i = 1; i <= len_a; ++i) {
        for (unsigned j = 1; j <= len_b; ++j) {
            idx_t cost = (a[i-1] == b[j-1]) ? 0 : 1;
            idx_t sub   = dp[i-1][j-1] + cost;
            idx_t ins   = dp[i][j-1]   + 1;
            idx_t del   = dp[i-1][j]   + 1;
            idx_t best  = sub;
            if (ins < best) best = ins;
            if (del < best) best = del;
            dp[i][j] = best;
        }
    }
    return (int)dp[len_a][len_b];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* array big enough for the binary representations of all n <= 10 000
       (needs at most 14 bits).  To be safe, use 32. */
    static uint8_t bits_n[32];
    static uint8_t bits_k[32];
    unsigned len_n = get_bits(n, bits_n);

    seq_t count = 0;

    for (idx_t k = 0; k <= n; ++k) {
        unsigned len_k = get_bits(k, bits_k);
        int l = levenshtein(bits_n, len_n, bits_k, len_k);
        int d = damerau_levenshtein(bits_n, len_n, bits_k, len_k);
        if (d < l)
            count = count + (seq_t)1;
    }
    return count;
}
