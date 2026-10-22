/*
 * A356898  a(n) is the number of trailing 1's in the maximal tribonacci
 *          representation of n (A352103).
 * https://oeis.org/A356898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of trailing 1's in the maximal tribonacci
 *            representation of n (A352103). The function computes this value
 *            by dynamic programming over the Tribonacci weights. It handles
 *            both fixed‑width integer builds and arbitrary‑precision builds;
 *            the DP tables use only unsigned char, so no dynami...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return the number of trailing 1's in the maximal tribonacci
   representation of n.  */
seq_t a(idx_t n)
{
    /* Trivial case */
    if (n == 0)
        return (seq_t)0;

    /* Build the Tribonacci sequence up to n (using 64‑bit arithmetic).
       We keep the values in a small array (at most ~70 elements for 64‑bit n). */
    uint64_t trib[128];
    size_t m = 0;
    if (n >= 1ULL)    trib[m++] = 1ULL;
    if (n >= 2ULL)    trib[m++] = 2ULL;
    if (n >= 4ULL)    trib[m++] = 4ULL;
    while (m >= 3) {
        unsigned __int128 sum = (unsigned __int128)trib[m-1] +
                               (unsigned __int128)trib[m-2] +
                               (unsigned __int128)trib[m-3];
        if (sum > (unsigned __int128)n)
            break;
        trib[m++] = (uint64_t)sum;
    }

    /* DP arrays: reach[s] tells if s is representable using processed
       weights; best[s] stores the maximal number of consecutive
       trailing 1's reachable for that sum.  */
    size_t sz = (size_t)n + 1;
    unsigned char *reach = (unsigned char *)calloc(sz, sizeof(unsigned char));
    unsigned char *best  = (unsigned char *)calloc(sz, sizeof(unsigned char));
    if (!reach || !best) {
        /* Allocation failed – fall back to 0 */
        free(reach);
        free(best);
        return (seq_t)0;
    }
    reach[0] = 1;          /* sum 0 is trivially reachable */
    best[0]  = 0;          /* no trailing 1's for sum 0 */

    /* Process weights in increasing order (from lowest to highest). */
    for (size_t i = 0; i < m; ++i) {
        uint64_t w64 = trib[i];
        size_t w = (size_t)w64;
        for (size_t s = sz - 1; s >= w; --s) {
            if (!reach[s - w])
                continue;
            unsigned char prevTra = best[s - w];
            unsigned char newTra = (prevTra == (unsigned char)i) ?
                                  (unsigned char)(i + 1) : prevTra;
            if (!reach[s] || newTra > best[s]) {
                reach[s] = 1;
                best[s]  = newTra;
            }
            if (s == w) break;   /* size_t is unsigned */
        }
    }

    seq_t result = (reach[(size_t)n]) ? (seq_t)best[(size_t)n] : (seq_t)0;
    free(reach);
    free(best);
    return result;
}
