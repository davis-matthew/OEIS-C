/*
 * A323373  Lexicographically earliest sequence such that for all i, j, a(i) =
 *          a(j) => A049559(i) = A049559(j) and A160595(i) = A160595(j).
 * https://oeis.org/A323373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the pairs
 *            [A049559(n), A160595(n)], where A049559(n) = gcd(n-1, phi(n))
 *            and A160595(n) = phi(n)/gcd(n-1, phi(n)). The code computes this
 *            by an incremental scan that numbers each distinct pair in order
 *            of first appearance
 * Range:     u8 n <= 380, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 118 terms through n = 9753 (2026-11-08)
 * Helpers:   gcd, mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/totient.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static uint64_t *val = NULL;   /* a(k) for k = 1..done */
    static size_t done = 0, cap = 0;
    static seq_t *pg = NULL, *pr = NULL;   /* distinct pairs, in order of first appearance */
    static size_t ndist = 0, dcap = 0;

    if (n < 1) return (seq_t)0;
    if ((size_t)n > done) {
        size_t need = (size_t)n;
        if (need > cap) {
            size_t nc = cap ? cap : 1024;
            while (nc < need) nc *= 2;
            val = (uint64_t *)realloc(val, nc * sizeof(uint64_t));
            cap = nc;
        }
        if (dcap == 0) {
            dcap = 1024;
            pg = mem_new(seq_t, dcap);
            pr = mem_new(seq_t, dcap);
        }
        for (size_t k = done + 1; k <= need; k++) {
            seq_t phi = totient((idx_t)k);
            seq_t km1 = (seq_t)k - 1;
            seq_t g = gcd(km1, phi);
            if (g == 0) g = 1;
            seq_t r = phi / g;
            size_t found = 0;
            for (size_t i = 0; i < ndist; i++) {
                if (pg[i] == g && pr[i] == r) { found = i + 1; break; }
            }
            if (!found) {
                if (ndist == dcap) {
                    dcap *= 2;
                    pg = mem_resize(seq_t, pg, dcap);
                    pr = mem_resize(seq_t, pr, dcap);
                }
                pg[ndist] = g;
                pr[ndist] = r;
                ndist++;
                found = ndist;
            }
            val[k - 1] = (uint64_t)found;
        }
        done = need;
    }
    return (seq_t)val[(size_t)n - 1];
}
