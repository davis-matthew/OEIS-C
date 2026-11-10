/*
 * A396014  Position of the first occurrence of n in A370470.
 * https://oeis.org/A396014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least m such that A370470(m) = n; computed by a DP search
 *            over the run lengths of m (the inverse of A370470's
 *            run-difference map)
 * Range:     u8 n <= 32, u16 n <= 520, u32 n <= 16704, u64 n <= 16704,
 *            u128 n <= 16704, big any n
 * Validated: standard, pass, 78 terms through n = 9784 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    int L = 0;
    {
        idx_t t = n;
        while (t) { L++; t >>= 1; }
    }
    int S = L + 1;
    size_t total = (size_t)(L + 1) * (size_t)S * 4;
    int *len = mem_new(int, total);
    seq_t *val = mem_new(seq_t, total);
    const int INF = 1 << 28;

    for (int pos = L; pos >= 0; --pos) {
        for (int z = 1; z >= 0; --z) {
            for (int last = 0; last <= L; ++last) {
                for (int par = 0; par < 2; ++par) {
                    size_t id = (((size_t)pos * S + last) * 2 + par) * 2 + z;
                    if (pos == L) {
                        len[id] = 0;
                        val[id] = (seq_t)0;
                        continue;
                    }
                    int bit = 1 - par;
                    int bestlen = INF;
                    seq_t bestval = (seq_t)0;
                    for (int s = 1; s <= L; ++s) {
                        int d = s > last ? s - last : last - s;
                        size_t nid;
                        if (d == 0) {
                            if (z == 1) continue;
                            nid = (((size_t)pos * S + last) * 2 + (1 - par)) * 2 + 1;
                        } else {
                            if (pos + d > L) continue;
                            bool ok = true;
                            for (int q = 0; q < d; ++q) {
                                int tb = (int)((n >> (L - 1 - (pos + q))) & 1);
                                if (tb != bit) { ok = false; break; }
                            }
                            if (!ok) continue;
                            nid = (((size_t)(pos + d) * S + s) * 2 + (1 - par)) * 2 + 0;
                        }
                        if (len[nid] >= INF) continue;
                        int cl = s + len[nid];
                        if (cl > bestlen) continue;
                        seq_t x = (seq_t)0;
                        for (int q = 0; q < s; ++q) x = (x << 1) | (seq_t)bit;
                        for (int q = 0; q < len[nid]; ++q) x = x << 1;
                        x = x | val[nid];
                        if (cl < bestlen || x < bestval) {
                            bestlen = cl;
                            bestval = x;
                        }
                    }
                    len[id] = bestlen;
                    val[id] = bestval;
                }
            }
        }
    }
    seq_t res = val[(((size_t)0 * S + 0) * 2 + 0) * 2 + 0];
    mem_free(len);
    mem_free(val);
    return res;
}
